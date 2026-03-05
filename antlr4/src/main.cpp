#include "AsgardParser.h"
#include "AsgardLexer.h"


const static std::map<std::string, std::string> primitive_types = {
	{"u8", "std::uint8_t"},
	{"i8", "std::int8_t"},
	{"u16", "std::uint16_t"},
	{"i16", "std::int16_t"},
	{"u32", "std::uint32_t"},
	{"i32", "std::int32_t"},
	{"u64", "std::uint64_t"},
	{"i64", "std::int64_t"},
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


int main(int argc, char **argv){
	if(argc != 2){
		std::cerr << "Usage: " << argv[0] << " file" << std::endl;
		return 1;
	}

	const std::string path = argv[1];
	std::ifstream stream(path);
	if(!stream){
		std::cerr << "Opening file " << path << " failed" << std::endl;
		return 1;
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
		std::cerr << "Lexing failed with: " << err.what() << std::endl;
		return 1;
	}

	AsgardParser parser(&tokens);
	parser.removeErrorListeners();
	parser.addErrorListener(&error_listener);
	AsgardParser::ClassdefContext *classdef = nullptr;
	AsgardParser::ModuledefContext *moduledef = nullptr;
	AsgardParser::EnumdefContext *enumdef = nullptr;
	try{
		std::string extension;
		const auto find_dot = path.rfind('.');
		if(find_dot != std::string::npos){
			extension = path.substr(find_dot+1);
		}
		if(extension == "data"){
			classdef = parser.classdef();
		}else if(extension == "module"){
			moduledef = parser.moduledef();
		}else if(extension == "enum"){
			enumdef = parser.enumdef();
		}
	}catch(const std::exception &err){
		std::cerr << "Parsing failed with: " << err.what() << std::endl;
		return 1;
	}

	if(classdef || moduledef){
		auto class_path = classdef ? classdef->extends()->class_path() : moduledef->extends()->class_path();
		std::cout << "extends " << get_class_path(class_path) << std::endl;
	}
	if(classdef){
		for(auto *member : classdef->member()){
			std::cout << get_declaration(member->declaration()) << std::endl;
		}
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
	if(classdef || moduledef){
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
	if(enumdef){
		for(auto *id : enumdef->ID()){
			std::cout << id->getSymbol()->getText() << "," << std::endl;
		}
	}

	return 0;
}

