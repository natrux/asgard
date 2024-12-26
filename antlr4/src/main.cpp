#include "AsgardParser.h"
#include "AsgardLexer.h"



class ThrowingErrorListener : public antlr4::BaseErrorListener{
public:
	void syntaxError(antlr4::Recognizer */*recognizer*/, antlr4::Token */*offendingSymbol*/, size_t line, size_t charPositionInLine, const std::string &msg, std::exception_ptr /*e*/) override{
		throw antlr4::ParseCancellationException("line " + std::to_string(line) + ":" + std::to_string(charPositionInLine) + " " + msg);
	}
};


static std::string get_classname(AsgardParser::ClassnameContext *context){
	std::string name;
	for(auto *id : context->ID()){
		if(!name.empty()){
			name += "::";
		}
		name += id->getSymbol()->getText();
	}
	return name;
}

static std::string get_type(AsgardParser::TypeContext *context, bool const_ref){
	if(context->QUESTION_MARK()){
		auto *type = context->type(0);
		const std::string name = "std::optional<" + get_type(type, false) + ">";
		if(const_ref){
			return "const " + name + "&";
		}
		return name;
	}else if(context->ASTERISK()){
		auto *type = context->type(0);
		return "std::shared_ptr<const " + get_type(type, false) + ">";
	}else if(context->LESS_THAN()){
		auto *base_class = context->classname();
		const auto types = context->type();
		std::string result = get_classname(base_class) + "<";
		bool is_first = true;
		for(auto *type : types){
			if(!is_first){
				result += ", ";
			}
			result += get_type(type, false);
			is_first = false;
		}
		result += ">";
		if(const_ref){
			result = "const " + result + "&";
		}
		return result;
	}
	const auto name = get_classname(context->classname());
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


int main(int argc, char **argv){
	if(argc != 2){
		std::cerr << "Usage: " << argv[0] << " module_file" << std::endl;
		return 1;
	}

	const std::string path = argv[1];
	std::ifstream stream(path);
	if(!stream){
		std::cerr << "Opening file " << path << " failed" << std::endl;
		return 1;
	}
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
	AsgardParser::ModuleContext *module;
	try{
		module = parser.module();
	}catch(const std::exception &err){
		std::cerr << "Parsing failed with: " << err.what() << std::endl;
		return 1;
	}

	std::cout << "extends " << get_classname(module->extends()->classname()) << std::endl;
	for(auto *member : module->member()){
		auto *declaration = member->declaration();
		std::cout << get_type(declaration->type(), false) << " " << declaration->ID()->getSymbol()->getText();
		if(auto *value = declaration->value()){
			std::cout << " = " << get_value(value);
		}
		std::cout << std::endl;
		if(member->MUTABLE()){
			// TODO
		}
	}
	for(auto *function : module->function()){
		std::cout
			<< "static "
			<< get_return_type(function->return_type())
			<< " "
			<< function->ID()->getSymbol()->getText()
			<< "("
			<< get_parameters(function->parameters()->declaration())
			<< ")"
		;
		std::cout << std::endl;
	}
	for(auto *method : module->method()){
		std::cout
			<< get_return_type(method->return_type())
			<< " "
			<< method->ID()->getSymbol()->getText()
			<< "("
			<< get_parameters(method->parameters()->declaration())
			<< ")"
		;
		if(method->CONST()){
			std::cout << " const";
		}
		std::cout << std::endl;
	}
	if(auto *handles = module->handles()){
		for(auto *classname : handles->classname()){
			std::cout << "void handle(std::shared_ptr<const " << get_classname(classname) << "> sample)" << std::endl;
		}
	}

	return 0;
}

