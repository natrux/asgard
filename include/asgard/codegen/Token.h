#pragma once

#include <string>
#include <vector>


namespace asgard{
namespace codegen{


enum class token_type_e{
	DOT,
	COMMA,
	EQUALS,
	PARENTHESIS_OPEN,
	PARENTHESIS_CLOSE,
	LESS_THAN,
	GREATER_THAN,
	ASTERISK,
	QUESTION_MARK,

	EXTENDS,
	CONST,
	STATIC,
	VOID,
	HANDLES,

	TRUE,
	FALSE,
	IDENTIFIER,
	INTEGER,
	FLOAT,
	STRING,
	DURATION,
};


class Token{
public:
	token_type_e type;
	size_t position;
	std::string content;

	Token() = default;
	Token(const token_type_e &type_, size_t position_, const std::string &content_):
		type(type_),
		position(position_),
		content(content_)
	{}
	Token(const token_type_e &type_, size_t position_, const char &content_):
		type(type_),
		position(position_),
		content({content_})
	{}
};


std::vector<Token> tokenize(const std::string &text);

}
}
