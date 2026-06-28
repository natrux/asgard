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


class Namespace{
public:
	Namespace &get(const std::string &name){
		auto &sub = subs[name];
		if(sub.trace.empty()){
			sub.trace = trace;
			sub.trace.push_back(name);
		}
		return sub;
	}

	const Namespace &get(const std::string &name) const{
		const auto find = subs.find(name);
		if(find == subs.end()){
			throw std::logic_error("No such namespace");
		}
		return find->second;
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
	std::vector<std::string> trace;
	std::map<std::string, Namespace> subs;
};


void parse_file(Namespace &ns, const std::string &path){
	std::ifstream stream(path);
	if(!stream){
		throw std::runtime_error("Opening file " + path + " failed");
	}

	const auto find_slash = path.rfind('/');
	const auto find_dot = path.rfind('.');
	const std::string name = path.substr(
		find_slash==std::string::npos ? 0 : find_slash+1,
		(find_dot==std::string::npos ? path.length() : find_dot) - (find_slash==std::string::npos ? 0 : find_slash) - 1
	);
	std::string extension;
	if(find_dot != std::string::npos){
		extension = path.substr(find_dot+1);
	}

	bool is_class = false;
	bool is_module = false;
	bool is_enum = false;
	if(extension == "data"){
		is_class = true;
	}else if(extension == "module"){
		is_module = true;
	}else if(extension == "enum"){
		is_enum = true;
	}else{
		throw std::logic_error("No file format recognized");
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
		if(is_class){
			classdef = parser.classdef();
		}else if(is_module){
			moduledef = parser.moduledef();
		}else if(is_enum){
			enumdef = parser.enumdef();
		}
	}catch(const std::exception &err){
		throw std::runtime_error("Parsing failed with: " + std::string(err.what()));
	}

	std::cout << "#pragma once" << std::endl;
	if(is_class || is_module){
	}else if(is_enum){
		std::cout << "#include <asgard/data/Enum.h>" << std::endl;
	}
	std::cout << "namespace " << ns.to_string("::") << "{" << std::endl;

	if(is_class){
		const auto extends = get_class_path(classdef ? classdef->extends()->class_path() : moduledef->extends()->class_path());
		std::cout << "class " << name << " : public " << extends << "{" << std::endl;
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
		std::cout << "\tstatic std::shared_ptr<" << name << "> create();" << std::endl;
		std::cout << "\t" << name << "();" << std::endl;
		std::cout << "\tvoid read_member(asgard::type::TypeReader &reader, const std::string &name, const asgard::type::Typecode &type) override;" << std::endl;
		std::cout << "\tvoid write_member(asgard::type::TypeWriter &writer, const std::string &name) const override;" << std::endl;
		std::cout << "\tvoid set_member(const std::string &name, const asgard::data::Bin &value) override;" << std::endl;
		std::cout << "\tasgard::data::Bin get_member(const std::string &name) const override;" << std::endl;
		std::cout << "};" << std::endl;
	}else if(is_module){
		std::cout << ns.to_string("::", name) << std::endl;
	}else if(is_enum){
		std::cout <<
			"class " << name << " : public asgard::data::Enum{" << std::endl <<
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
			"\t" << name << "() = default;" << std::endl <<
			"\t" << name << "(const enum_e &v);" << std::endl <<
			"\t" << name << " &operator=(const " << name << " &other) = default;" << std::endl <<
			"\t" << name << " &operator=(const enum_e &v);" << std::endl <<
			std::endl <<
			"\tasgard::type::EnumMap enum_map() const override;" << std::endl <<
			"\tvoid from_int(const asgard::type::enum_t &v) override;" << std::endl <<
			"\ttype::enum_t to_int() const override;" << std::endl <<
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

	std::cout << "#include <" << ns.to_string("/", name) << ".hxx>" << std::endl;
	std::cout << "namespace " << ns.to_string("::") << "{" << std::endl;

	if(is_class){
		std::vector<std::string> members;
		for(auto *member : classdef->member()){
			const std::string name = member->declaration()->ID()->getSymbol()->getText();
			members.push_back(name);
		}
		std::cout << "asgard::type::Signature " << name << "::static_signature(){" << std::endl;
		std::cout << "\tauto sig = Super::static_signature();" << std::endl;
		std::cout << "\tsig.parents.push_back(sig.name);" << std::endl;
		std::cout << "\tsig.name = \"" << ns.to_string(".", name) << "\";" << std::endl;
		for(const auto &member : members){
			std::cout << "\tsig.members[\"" << member << "\"] = type::get_typecode<decltype(" << member << ")>();" << std::endl;
		}
		std::cout << "\treturn sig;" << std::endl;
		std::cout << "}" << std::endl;
		std::cout << "std::shared_ptr<" << name << "> " << name << "::create(){" << std::endl;
		std::cout << "\treturn std::make_shared<" << name << ">();" << std::endl;
		std::cout << "}" << std::endl;
		std::cout << name << "::" << name << "(){" << std::endl;
		std::cout << "\tset_signature(static_signature());" << std::endl;
		std::cout << "}" << std::endl;
		std::cout << "void " << name << "::read_member(asgard::type::TypeReader &reader, const std::string &name, const asgard::type::Typecode &type){" << std::endl;
		std::cout << "\tif(false){" << std::endl;
		for(const auto &member : members){
			std::cout << "\t}else if(name == \"" << member << "\"){" << std::endl;
			std::cout << "\t\treader.read_type(" << member << ", type);" << std::endl;
		}
		std::cout << "\t}else{" << std::endl;
		std::cout << "\t\tSuper::read_member(reader, name, type);" << std::endl;
		std::cout << "\t}" << std::endl;
		std::cout << "}" << std::endl;
		std::cout << "void " << name << "::write_member(asgard::type::TypeWriter &writer, const std::string &name) const{" << std::endl;
		std::cout << "\tif(false){" << std::endl;
		for(const auto &member : members){
			std::cout << "\t}else if(name == \"" << member << "\"){" << std::endl;
			std::cout << "\t\twriter.write_value(" << member << ");" << std::endl;
		}
		std::cout << "\t}else{" << std::endl;
		std::cout << "\t\tSuper::write_member(writer, name);" << std::endl;
		std::cout << "\t}" << std::endl;
		std::cout << "}" << std::endl;
		std::cout << "void " << name << "::set_member(const std::string &name, const asgard::data::Bin &value){" << std::endl;
		std::cout << "\tif(false){" << std::endl;
		for(const auto &member : members){
			std::cout << "\t}else if(name == \"" << member << "\"){" << std::endl;
			std::cout << "\t\tvalue.to(" << member << ");" << std::endl;
		}
		std::cout << "\t}else{" << std::endl;
		std::cout << "\t\tSuper::set_member(name, value);" << std::endl;
		std::cout << "\t}" << std::endl;
		std::cout << "}" << std::endl;
		std::cout << "asgard::data::Bin " << name << "::get_member(const std::string &name) const{" << std::endl;
		std::cout << "\tif(false){" << std::endl;
		for(const auto &member : members){
			std::cout << "\t}else if(name == \"" << member << "\"){" << std::endl;
			std::cout << "\t\treturn " << member << ";" << std::endl;
		}
		std::cout << "\t}else{" << std::endl;
		std::cout << "\t\treturn Super::get_member(name);" << std::endl;
		std::cout << "\t}" << std::endl;
		std::cout << "}" << std::endl;
		std::cout << "static asgard::core::register_type_t<" << name << "> register_type;" << std::endl;
	}if(is_module){
	}else if(is_enum){
		std::cout << "const std::map<type::enum_t, std::string> " << name << "::_enum_map = {" << std::endl;
		for(auto *id : enumdef->ID()){
			const auto text = id->getSymbol()->getText();
			std::cout << "\t{" << text << ", \"" << text << "\"}," << std::endl;
		}
		std::cout << "};" << std::endl;

		std::cout <<
			"type::EnumMap " << name << "::static_enum_map(){" << std::endl <<
			"\ttype::EnumMap map;" << std::endl <<
			"\tmap.name = \"" << ns.to_string(".", name) << "\";" << std::endl <<
			"\tmap.enum_map = _enum_map;" << std::endl <<
			"\tmap.fill_reverse();" << std::endl <<
			"\treturn map;" << std::endl <<
			"}" << std::endl <<
		std::endl;

		std::cout <<
			name << "::" << name << "(const enum_e &v):" << std::endl <<
			"\tvalue(v)" << std::endl <<
			"{" << std::endl <<
			"}" << std::endl <<
		std::endl;

		std::cout <<
			name << " &" << name << "::operator=(const enum_e &v){" << std::endl <<
			"\tvalue = v;" << std::endl <<
			"\treturn *this;" << std::endl <<
			"}" << std::endl <<
		std::endl;

		std::cout <<
			"type::EnumMap " << name << "::enum_map() const{" << std::endl <<
			"\treturn static_enum_map();" << std::endl <<
			"}" << std::endl <<
		std::endl;

		std::cout <<
			"void " << name << "::from_int(const type::enum_t &v){" << std::endl <<
			"\tconst auto find = _enum_map.find(v);" << std::endl <<
			"\tif(find == _enum_map.end()){" << std::endl <<
			"\t\tvalue = zero;" << std::endl <<
			"\t}else{" << std::endl <<
			"\t\tvalue = static_cast<enum_e>(v);" << std::endl <<
			"\t}" << std::endl <<
			"}" << std::endl <<
		std::endl;

		std::cout <<
			"type::enum_t " << name << "::to_int() const{" << std::endl <<
			"\treturn value;" << std::endl <<
			"}" << std::endl <<
		std::endl;

		std::cout <<
			name << "::operator enum_e() const{" << std::endl <<
			"\treturn value;" << std::endl <<
			"}" << std::endl <<
		std::endl;
	}

	std::cout << "}" << std::endl;
}





void parse_directory(Namespace &ns, const std::string &path){
	std::vector<std::string> files;
	std::vector<std::string> directories;
	util::ls(path, files, directories);
	for(const auto &file : files){
		const std::string full_path = path + "/" + file;
		std::cout << "=== " << full_path << " ===" << std::endl;
		try{
			asgard::code::parse_file(ns, full_path);
		}catch(const std::exception &err){
			std::cerr << err.what() << std::endl;
		}
	}
	for(const auto &dir : directories){
		const std::string subdir = path + "/" + dir;
		parse_directory(ns.get(dir), subdir);
	}
}


void parse_packages(const std::vector<std::string> &paths){
	Namespace root;
	for(const auto &path : paths){
		parse_directory(root, path);
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

