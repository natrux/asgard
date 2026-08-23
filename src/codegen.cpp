#include <asgard/code/AsgardParser.h>
#include <asgard/code/AsgardLexer.h>
#include <asgard/util/ls.h>
#include <asgard/util/crc_32_iso_hdlc.h>
#include <asgard/util/crc_64_xz.h>


namespace asgard{
namespace code{


const static std::map<std::string, std::string> primitive_types = {
	{"u8", "std::uint8_t"},
	{"i8", "std::int8_t"},
	{"u16", "std::uint16_t"},
	{"i16", "std::int16_t"},
	{"u32", "std::uint32_t"},
	{"i32", "std::int32_t"},
	{"u64", "std::uint64_t"},
	{"i64", "std::int64_t"},
	{"f32", "float"},
	{"f64", "double"},
	{"string", "std::string"},
};


class ThrowingErrorListener : public antlr4::BaseErrorListener{
public:
	void syntaxError(antlr4::Recognizer */*recognizer*/, antlr4::Token */*offendingSymbol*/, size_t line, size_t charPositionInLine, const std::string &msg, std::exception_ptr /*e*/) override{
		throw antlr4::ParseCancellationException("line " + std::to_string(line) + ":" + std::to_string(charPositionInLine) + " " + msg);
	}
};


static std::string get_class_path(AsgardParser::Class_pathContext *context){
	std::string name;
	if(auto *name_space = context->name_space()){
		for(auto *id : name_space->ID()){
			if(!name.empty()){
				name += "::";
			}
			name += id->getSymbol()->getText();
		}
		name += "::";
	}
	name += context->ID()->getSymbol()->getText();

	return name;
}

static std::string get_type(AsgardParser::TypeContext *context, bool const_ref){
	if(context->ASTERISK()){
		// special case for no const-ref
		auto *type = context->type(0);
		return "std::shared_ptr<const " + get_type(type, false) + ">";
	}
	std::string name;
	if(context->QUESTION_MARK()){
		auto *type = context->type(0);
		name = "std::optional<" + get_type(type, false) + ">";
	}else if(context->PARENTHESIS_OPEN()){
		const auto types = context->type();
		const auto size = types.size();
		if(size == 1){
			name = "std::unordered_set<" + get_type(types[0], false) + ">";
		}else if(size == 2){
			name = "std::unordered_map<" + get_type(types[0], false) + ", " + get_type(types[1], false) + ">";
		}
	}else if(context->BRACKET_OPEN()){
		auto *type = context->type(0);
		if(auto *number = context->NATURAL()){
			name = "std::array<" + get_type(type, false) + ", " + number->getSymbol()->getText() + ">";
		}else{
			name = "std::vector<" + get_type(type, false) + ">";
		}
	}else if(context->BRACE_OPEN()){
		const auto types = context->type();
		const auto size = types.size();
		if(size == 1){
			name = "std::set<" + get_type(types[0], false) + ">";
		}else if(size == 2){
			name = "std::map<" + get_type(types[0], false) + ", " + get_type(types[1], false) + ">";
		}
	}else if(auto *primitive_type = context->primitive_type()){
		const auto type_name = primitive_type->start->getText();
		const auto find = primitive_types.find(type_name);
		if(find == primitive_types.end()){
			throw std::logic_error("Primitive type '" + type_name + "' not found");
		}
		name = find->second;
	}else if(context->LESS_THAN()){
		const auto types = context->type();
		if(types.size() == 2){
			name = "std::pair<" + get_type(types[0], false) + ", " + get_type(types[1], false) + ">";
		}else{
			name = "std::tuple<";
			bool first = true;
			for(auto type : types){
				if(!first){
					name += ", ";
				}
				name += get_type(type, false);
				first = false;
			}
			name += ">";
		}
	}else{
		name = get_class_path(context->class_path());
	}
	if(const_ref){
		return "const " + name + "&";
	}
	return name;
}

static std::string get_return_type(AsgardParser::Return_typeContext *context){
	if(context->VOID()){
		return "void";
	}
	return get_type(context->type(), false);
}

static std::string get_value(AsgardParser::ValueContext *context){
	if(auto *boool = context->BOOL()){
		return boool->getSymbol()->getText();
	}else if(auto *number = context->NUMBER()){
		return number->getSymbol()->getText();
	}else if(auto *string = context->STRING()){
		return string->getSymbol()->getText();
	}else if(auto *duration = context->duration()){
		std::string result;
		bool is_first = true;
		for(auto *unit : duration->duration_unit()){
			if(!is_first){
				result += " + ";
			}
			result += unit->getText();
			is_first = false;
		}
		return result;
	}
	throw std::logic_error("Unrecognized value");
}

static std::string get_parameters(const std::vector<AsgardParser::DeclarationContext *> &parameters){
	std::string result;
	bool first_parameter = true;
	for(auto *parameter : parameters){
		if(!first_parameter){
			result += ", ";
		}
		result += get_type(parameter->type(), true) + " " + parameter->ID()->getSymbol()->getText();
		if(auto *value = parameter->value()){
			result += "=" + get_value(value);
		}
		first_parameter = false;
	}
	return result;
}

static std::string get_function(AsgardParser::FunctionContext *function){
	return "static " + get_return_type(function->return_type()) + " " + function->ID()->getSymbol()->getText() + "(" + get_parameters(function->parameters()->declaration()) + ")";
}

static std::string get_method(AsgardParser::MethodContext *method){
	std::string result = get_return_type(method->return_type()) + " " + method->ID()->getSymbol()->getText() + "(" + get_parameters(method->parameters()->declaration()) + ")";
	if(method->CONST()){
		result += " const";
	}
	return result;
}

static std::string get_declaration(AsgardParser::DeclarationContext *declaration){
	std::string result = get_type(declaration->type(), false) + " " + declaration->ID()->getSymbol()->getText();
	if(auto *value = declaration->value()){
		result += " = " + get_value(value);
	}
	return result;
}


class Namespace;

class Type{
public:
	std::string name;
	std::weak_ptr<Namespace> ns;

	bool is_class = false;
	bool is_module = false;
	bool is_enum = false;
	bool is_builtin = false;
	bool is_extern = false;

	virtual ~Type() = default;
	virtual std::string get_include_path() const = 0;
};

class Generated : public Type{
public:
	std::string def_file;

	std::string get_include_path() const override;
};

class Class : public Generated{
public:
	Class(){ is_class = true; }
};

class Module : public Generated{
public:
	Module(){ is_module = true; }
};

class Enum : public Generated{
public:
	Enum(){ is_enum = true; }
};

class Builtin : public Type{
public:
	std::string include_file;

	Builtin(){ is_builtin = true; }

	std::string get_include_path() const override;
};

class Extern : public Type{
public:
	std::string include_file;

	Extern(){ is_extern = true; }

	std::string get_include_path() const override;
};

class Namespace{
public:
	static std::shared_ptr<Namespace> create(){
		auto ptr = std::make_shared<Namespace>();
		ptr->self = ptr;
		return ptr;
	}

	void add(std::shared_ptr<Type> type){
		types[type->name] = type;
		type->ns = self;
	}

	std::shared_ptr<Type> find(const std::string &name, const std::vector<std::string> &ns) const{
		std::shared_ptr<Type> result;
		if(ns.empty()){
			const auto find = types.find(name);
			if(find != types.end()){
				result = find->second;
			}
		}else{
			const auto found = subnames.find(ns.front());
			if(found != subnames.end()){
				auto sub = found->second;
				const std::vector<std::string> ns_tail(++ns.begin(), ns.end());
				result = sub->find(name, ns_tail);
			}
		}
		if(!result){
			if(auto p = parent.lock()){
				result = p->find(name, ns);
			}
		}
		return result;
	}

	std::shared_ptr<Type> find(const std::string &name){
		return find(name, {});
	}

	std::vector<std::shared_ptr<Type>> get_all() const{
		std::vector<std::shared_ptr<Type>> result;
		for(const auto &entry : types){
			result.push_back(entry.second);
		}
		for(const auto &entry : subnames){
			const auto ts = entry.second->get_all();
			result.insert(result.end(), ts.begin(), ts.end());
		}
		return result;
	}

	std::shared_ptr<Namespace> sub(const std::string &name){
		auto &sub = subnames[name];
		if(!sub){
			sub = create();
			sub->parent = self.lock();
			sub->trace = trace;
			sub->trace.push_back(name);
		}
		return sub;
	}

	std::string to_string(const std::string &sep) const{
		std::string result;
		for(const auto &name : trace){
			if(!result.empty()){
				result.append(sep);
			}
			result.append(name);
		}
		return result;
	}

	std::string to_string(const std::string &sep, const std::string &name) const{
		std::string result = to_string(sep);
		if(!result.empty()){
			result.append(sep);
		}
		result.append(name);
		return result;
	}

private:
	std::weak_ptr<Namespace> self;
	std::weak_ptr<Namespace> parent;
	std::vector<std::string> trace;
	std::map<std::string, std::shared_ptr<Type>> types;
	std::map<std::string, std::shared_ptr<Namespace>> subnames;
};


std::string Generated::get_include_path() const{
	return ns.lock()->to_string("/", name+".hxx");
}

std::string Builtin::get_include_path() const{
	return ns.lock()->to_string("/", include_file);
}

std::string Extern::get_include_path() const{
	return ns.lock()->to_string("/", include_file);
}






void parse_file(const Generated &type){
	auto ns = type.ns.lock();
	if(!ns){
		throw std::logic_error("Type has no namespace");
	}
	std::ifstream stream(type.def_file);
	if(!stream){
		throw std::runtime_error("Opening file " + type.def_file + " failed");
	}

	AsgardLexer::initialize();
	AsgardParser::initialize();
	antlr4::ANTLRInputStream input(stream);
	ThrowingErrorListener error_listener;

	AsgardLexer lexer(&input);
	lexer.removeErrorListeners();
	lexer.addErrorListener(&error_listener);
	antlr4::CommonTokenStream tokens(&lexer);
	try{
		tokens.fill();
	}catch(const std::exception &err){
		throw std::runtime_error("Lexing failed with: " + std::string(err.what()));
	}

	AsgardParser parser(&tokens);
	parser.removeErrorListeners();
	parser.addErrorListener(&error_listener);
	AsgardParser::ClassdefContext *classdef = nullptr;
	AsgardParser::ModuledefContext *moduledef = nullptr;
	AsgardParser::EnumdefContext *enumdef = nullptr;
	try{
		if(type.is_class){
			classdef = parser.classdef();
		}else if(type.is_module){
			moduledef = parser.moduledef();
		}else if(type.is_enum){
			enumdef = parser.enumdef();
		}
	}catch(const std::exception &err){
		throw std::runtime_error("Parsing failed with: " + std::string(err.what()));
	}

	std::cout << "#pragma once" << std::endl;
	if(type.is_class || type.is_module){
	}else if(type.is_enum){
		std::cout << "#include <asgard/data/Enum.h>" << std::endl;
	}
	std::cout << "namespace " << ns->to_string("::") << "{" << std::endl;

	if(type.is_class){
		const auto extends = get_class_path(classdef ? classdef->extends()->class_path() : moduledef->extends()->class_path());
		std::cout << "class " << type.name << " : public " << extends << "{" << std::endl;
		std::cout << "\tusing Super = " << extends << ";" << std::endl;
		std::cout << "public:" << std::endl;
		for(auto *member : classdef->member()){
			std::cout << "\t" << get_declaration(member->declaration()) << ";" << std::endl;
		}
		for(auto *function : classdef->function()){
			std::cout << "\t" << get_function(function) << std::endl;
		}
		for(auto *method : classdef->method()){
			std::cout << "\t" << get_method(method) << std::endl;
		}
		std::cout << "\tstatic asgard::type::Signature static_signature();" << std::endl;
		std::cout << "\tstatic std::shared_ptr<" << type.name << "> create();" << std::endl;
		std::cout << "\t" << type.name << "();" << std::endl;
		std::cout << "\tvoid read_member(asgard::type::TypeReader &reader, const std::string &name, const asgard::type::Typecode &type) override;" << std::endl;
		std::cout << "\tvoid write_member(asgard::type::TypeWriter &writer, const std::string &name) const override;" << std::endl;
		std::cout << "\tvoid set_member(const std::string &name, const asgard::data::Bin &value) override;" << std::endl;
		std::cout << "\tasgard::data::Bin get_member(const std::string &name) const override;" << std::endl;
		std::cout << "};" << std::endl;
	}else if(type.is_module){
		std::cout << ns->to_string("::", type.name) << std::endl;
	}else if(type.is_enum){
		std::cout <<
			"class " << type.name << " : public asgard::data::Enum{" << std::endl <<
			"public:" << std::endl <<
			"\tenum enum_e : asgard::type::enum_t{" << std::endl
		;
		for(auto *id : enumdef->ID()){
			const auto text = id->getSymbol()->getText();
			std::cout << "\t\t" << text << " = 0x" << std::hex << util::CRC_32_ISO_HDLC::compute(text.c_str(), text.length()) << std::dec << "," << std::endl;
		}
		std::cout << "\t};" << std::endl;
		std::cout <<
			"\tstatic asgard::type::EnumMap static_enum_map();" << std::endl <<
			"\t" << type.name << "() = default;" << std::endl <<
			"\t" << type.name << "(const enum_e &v);" << std::endl <<
			"\t" << type.name << " &operator=(const " << type.name << " &other) = default;" << std::endl <<
			"\t" << type.name << " &operator=(const enum_e &v);" << std::endl <<
			std::endl <<
			"\tasgard::type::EnumMap enum_map() const override;" << std::endl <<
			"\tvoid from_int(const asgard::type::enum_t &v) override;" << std::endl <<
			"\tasgard::type::enum_t to_int() const override;" << std::endl <<
			std::endl <<
			"\toperator enum_e() const;" << std::endl <<
		std::endl;
		std::cout << "private:" << std::endl;
		std::cout <<
			"\tstatic const enum_e zero = static_cast<enum_e>(0);" << std::endl <<
			"\tstatic const std::map<asgard::type::enum_t, std::string> _enum_map;" << std::endl <<
			"\tenum_e value = zero;" << std::endl <<
		std::endl;

		std::cout << "};" << std::endl;
	}

	if(moduledef){
		auto class_path = classdef ? classdef->extends()->class_path() : moduledef->extends()->class_path();
		std::cout << "extends " << get_class_path(class_path) << std::endl;
	}
	if(moduledef){
		for(auto *member : moduledef->module_member()){
			auto *declaration = member->declaration();
			std::cout << get_declaration(declaration) << std::endl;
			if(member->MUTABLE()){
				std::cout << "virtual void mutate_" << declaration->ID()->getSymbol()->getText() << "(" << get_type(declaration->type(), true) << " new_value) = 0;" << std::endl;
			}
		}
	}
	if(moduledef){
		for(auto *function : classdef ? classdef->function() : moduledef->function()){
			std::cout << get_function(function) << std::endl;
		}
		for(auto *method : classdef ? classdef->method() : moduledef->method()){
			std::cout << get_method(method) << std::endl;
		}
	}
	if(moduledef){
		if(auto *handles = moduledef->handles()){
			for(auto *class_path : handles->class_path()){
				std::cout << "void handle(std::shared_ptr<const " << get_class_path(class_path) << "> sample)" << std::endl;
			}
		}
	}

	std::cout << "}" << std::endl;

	std::cout << "#include <" << type.get_include_path() << ">" << std::endl;
	std::cout << "namespace " << ns->to_string("::") << "{" << std::endl;

	if(type.is_class){
		std::vector<std::string> members;
		for(auto *member : classdef->member()){
			const std::string name = member->declaration()->ID()->getSymbol()->getText();
			members.push_back(name);
		}
		std::cout << "asgard::type::Signature " << type.name << "::static_signature(){" << std::endl;
		std::cout << "\tauto sig = Super::static_signature();" << std::endl;
		std::cout << "\tsig.parents.push_back(sig.name);" << std::endl;
		std::cout << "\tsig.name = \"" << ns->to_string(".", type.name) << "\";" << std::endl;
		for(const auto &member : members){
			std::cout << "\tsig.members[\"" << member << "\"] = type::get_typecode<decltype(" << member << ")>();" << std::endl;
		}
		std::cout << "\treturn sig;" << std::endl;
		std::cout << "}" << std::endl;
		std::cout << "std::shared_ptr<" << type.name << "> " << type.name << "::create(){" << std::endl;
		std::cout << "\treturn std::make_shared<" << type.name << ">();" << std::endl;
		std::cout << "}" << std::endl;
		std::cout << type.name << "::" << type.name << "(){" << std::endl;
		std::cout << "\tset_signature(static_signature());" << std::endl;
		std::cout << "}" << std::endl;
		std::cout << "void " << type.name << "::read_member(asgard::type::TypeReader &reader, const std::string &name, const asgard::type::Typecode &type){" << std::endl;
		std::cout << "\tif(false){" << std::endl;
		for(const auto &member : members){
			std::cout << "\t}else if(name == \"" << member << "\"){" << std::endl;
			std::cout << "\t\treader.read_type(" << member << ", type);" << std::endl;
		}
		std::cout << "\t}else{" << std::endl;
		std::cout << "\t\tSuper::read_member(reader, name, type);" << std::endl;
		std::cout << "\t}" << std::endl;
		std::cout << "}" << std::endl;
		std::cout << "void " << type.name << "::write_member(asgard::type::TypeWriter &writer, const std::string &name) const{" << std::endl;
		std::cout << "\tif(false){" << std::endl;
		for(const auto &member : members){
			std::cout << "\t}else if(name == \"" << member << "\"){" << std::endl;
			std::cout << "\t\twriter.write_value(" << member << ");" << std::endl;
		}
		std::cout << "\t}else{" << std::endl;
		std::cout << "\t\tSuper::write_member(writer, name);" << std::endl;
		std::cout << "\t}" << std::endl;
		std::cout << "}" << std::endl;
		std::cout << "void " << type.name << "::set_member(const std::string &name, const asgard::data::Bin &value){" << std::endl;
		std::cout << "\tif(false){" << std::endl;
		for(const auto &member : members){
			std::cout << "\t}else if(name == \"" << member << "\"){" << std::endl;
			std::cout << "\t\tvalue.to(" << member << ");" << std::endl;
		}
		std::cout << "\t}else{" << std::endl;
		std::cout << "\t\tSuper::set_member(name, value);" << std::endl;
		std::cout << "\t}" << std::endl;
		std::cout << "}" << std::endl;
		std::cout << "asgard::data::Bin " << type.name << "::get_member(const std::string &name) const{" << std::endl;
		std::cout << "\tif(false){" << std::endl;
		for(const auto &member : members){
			std::cout << "\t}else if(name == \"" << member << "\"){" << std::endl;
			std::cout << "\t\treturn " << member << ";" << std::endl;
		}
		std::cout << "\t}else{" << std::endl;
		std::cout << "\t\treturn Super::get_member(name);" << std::endl;
		std::cout << "\t}" << std::endl;
		std::cout << "}" << std::endl;
		std::cout << "static asgard::core::register_type_t<" << type.name << "> register_type;" << std::endl;
	}if(type.is_module){
	}else if(type.is_enum){
		std::cout << "const std::map<asgard::type::enum_t, std::string> " << type.name << "::_enum_map = {" << std::endl;
		for(auto *id : enumdef->ID()){
			const auto text = id->getSymbol()->getText();
			std::cout << "\t{" << text << ", \"" << text << "\"}," << std::endl;
		}
		std::cout << "};" << std::endl;

		std::cout <<
			"asgard::type::EnumMap " << type.name << "::static_enum_map(){" << std::endl <<
			"\tasgard::type::EnumMap map;" << std::endl <<
			"\tmap.name = \"" << ns->to_string(".", type.name) << "\";" << std::endl <<
			"\tmap.enum_map = _enum_map;" << std::endl <<
			"\tmap.fill_reverse();" << std::endl <<
			"\treturn map;" << std::endl <<
			"}" << std::endl <<
		std::endl;

		std::cout <<
			type.name << "::" << type.name << "(const enum_e &v):" << std::endl <<
			"\tvalue(v)" << std::endl <<
			"{" << std::endl <<
			"}" << std::endl <<
		std::endl;

		std::cout <<
			type.name << " &" << type.name << "::operator=(const enum_e &v){" << std::endl <<
			"\tvalue = v;" << std::endl <<
			"\treturn *this;" << std::endl <<
			"}" << std::endl <<
		std::endl;

		std::cout <<
			"asgard::type::EnumMap " << type.name << "::enum_map() const{" << std::endl <<
			"\treturn static_enum_map();" << std::endl <<
			"}" << std::endl <<
		std::endl;

		std::cout <<
			"void " << type.name << "::from_int(const asgard::type::enum_t &v){" << std::endl <<
			"\tconst auto find = _enum_map.find(v);" << std::endl <<
			"\tif(find == _enum_map.end()){" << std::endl <<
			"\t\tvalue = zero;" << std::endl <<
			"\t}else{" << std::endl <<
			"\t\tvalue = static_cast<enum_e>(v);" << std::endl <<
			"\t}" << std::endl <<
			"}" << std::endl <<
		std::endl;

		std::cout <<
			"asgard::type::enum_t " << type.name << "::to_int() const{" << std::endl <<
			"\treturn value;" << std::endl <<
			"}" << std::endl <<
		std::endl;

		std::cout <<
			type.name << "::operator enum_e() const{" << std::endl <<
			"\treturn value;" << std::endl <<
			"}" << std::endl <<
		std::endl;
	}

	std::cout << "}" << std::endl;
}





void search_directory(std::shared_ptr<Namespace> ns, const std::string &path){
	std::vector<std::string> files;
	std::vector<std::string> directories;
	util::ls(path, files, directories);
	for(const auto &file : files){
		const std::string full_path = path + "/" + file;
		std::cout << "=== " << full_path << " ===" << std::endl;

		const auto find_dot = file.rfind('.');
		const std::string name = file.substr(0, find_dot);
		std::string extension;
		if(find_dot != std::string::npos){
			extension = file.substr(find_dot+1);
		}

		std::shared_ptr<Generated> type;
		if(extension == "data"){
			type = std::make_shared<Class>();
		}else if(extension == "module"){
			type = std::make_shared<Module>();
		}else if(extension == "enum"){
			type = std::make_shared<Enum>();
		}else{
			continue;
		}
		type->name = name;
		type->def_file = full_path;
		ns->add(type);
	}
	for(const auto &dir : directories){
		const std::string subdir = path + "/" + dir;
		search_directory(ns->sub(dir), subdir);
	}
}


void parse_packages(const std::vector<std::string> &paths){
	auto root = Namespace::create();
	{
		auto ns = root->sub("asgard")->sub("core");
		{
			auto type = std::make_shared<Builtin>();
			type->name = "ID";
			type->include_file = "ID.h";
			ns->add(type);
		}
	}
	{
		auto ns = root->sub("asgard")->sub("data");
		{
			auto type = std::make_shared<Builtin>();
			type->name = "Value";
			type->include_file = "Value.h";
			ns->add(type);
		}
	}
	{
		auto ns = root->sub("asgard")->sub("topic");
		{
			auto type = std::make_shared<Builtin>();
			type->name = "TopicPtr";
			type->include_file = "TopicPtr.h";
			ns->add(type);
		}
	}
	{
		auto ns = root->sub("asgard")->sub("mod");
		{
			auto type = std::make_shared<Builtin>();
			type->name = "Module";
			type->include_file = "Module.h";
			ns->add(type);
		}
	}
	{
		auto ns = root->sub("asgard")->sub("time");
		{
			auto type = std::make_shared<Builtin>();
			type->name = "time";
			type->include_file = "time.h";
			ns->add(type);
		}
		{
			auto type = std::make_shared<Builtin>();
			type->name = "wall_time";
			type->include_file = "time.h";
			ns->add(type);
		}
		{
			auto type = std::make_shared<Builtin>();
			type->name = "duration";
			type->include_file = "time.h";
			ns->add(type);
		}
	}

	for(const auto &path : paths){
		search_directory(root, path);
	}

	const auto types = root->get_all();
	for(auto type : types){
		if(type->is_class || type->is_module || type->is_enum){
			auto generated_type = std::static_pointer_cast<Generated>(type);
			try{
				asgard::code::parse_file(*generated_type);
			}catch(const std::exception &err){
				std::cerr << err.what() << std::endl;
			}
		}
	}
}


}
}


int main(int argc, char **argv){
	std::vector<std::string> paths;
	for(int i=1; i<argc; i++){
		paths.push_back(argv[i]);
	}
	asgard::code::parse_packages(paths);

	return 0;
}

