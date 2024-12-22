#include "AsgardParser.h"
#include "AsgardLexer.h"
#include "AsgardBaseListener.h"
#include "AsgardBaseVisitor.h"



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

static std::string get_type(AsgardParser::TypeContext *context){
	// TODO
	return "[Type]";
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

	/*for (auto token : tokens.getTokens()) {
		std::cout << token->toString() << std::endl;
	}*/

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
	for(auto *method : module->method()){
		auto *return_type = method->return_type();
		if(return_type->VOID()){
			std::cout << "void";
		}else{
			std::cout << get_type(return_type->type());
		}
		const auto name = method->ID()->getSymbol()->getText();
		std::cout << " " << name << "(";
		const auto parameters = method->parameters()->declaration();
		bool first_parameter = true;
		for(auto *parameter : parameters){
			if(!first_parameter){
				std::cout << ", ";
			}
			std::cout << get_type(parameter->type()) << " " << parameter->ID()->getSymbol()->getText();
			first_parameter = false;
		}
		std::cout << ")";
		if(method->CONST()){
			std::cout << " const";
		}
		std::cout << std::endl;
	}
	auto *handles = module->handles();
	if(handles){
		for(auto *classname : handles->classname()){
			std::cout << "void handle(std::shared_ptr<const " << get_classname(classname) << "> sample);" << std::endl;
		}
	}

	//Listener listener;
	//antlr4::tree::ParseTreeWalker::DEFAULT.walk(&listener, tree);

	//Visitor visitor;
	//tree->accept(&visitor);

	return 0;
}

