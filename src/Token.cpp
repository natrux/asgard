#include <asgard/codegen/Token.h>

#include <map>
#include <stdexcept>


namespace asgard{
namespace codegen{


static std::map<char, token_type_e> one_char_lookup = {
	{'.', token_type_e::DOT},
	{',', token_type_e::COMMA},
	{'=', token_type_e::EQUALS},
	{'(', token_type_e::PARENTHESIS_OPEN},
	{')', token_type_e::PARENTHESIS_CLOSE},
	{'<', token_type_e::LESS_THAN},
	{'>', token_type_e::GREATER_THAN},
	{'*', token_type_e::ASTERISK},
	{'?', token_type_e::QUESTION_MARK},
};

static std::map<std::string, token_type_e> keyword_lookup = {
	{"true", token_type_e::TRUE},
	{"false", token_type_e::FALSE},
	{"extends", token_type_e::EXTENDS},
	{"const", token_type_e::CONST},
	{"static", token_type_e::STATIC},
	{"void", token_type_e::VOID},
	{"handles", token_type_e::HANDLES},
	{"processes", token_type_e::HANDLES},
};

static bool is_whitespace(const char &chr){
	return
		chr == ' ' ||
		chr == '\n' ||
		chr == '\t' ||
		chr == '\r' ||
	false;
}

static bool is_alphanumeric(const char &chr){
	return
		(chr >= '0' && chr <= '9') ||
		(chr >= 'a' && chr <= 'z') ||
		(chr >= 'A' && chr <= 'Z') ||
		chr == '_' ||
	false;
}


std::vector<Token> tokenize(const std::string &text){
	std::vector<Token> result;

	size_t position = 0;
	while(position < text.length()){
		const size_t remaining = text.length() - position;
		const auto &chr = text.at(position);

		if(is_whitespace(chr)){
			position++;
			continue;
		}

		const auto find_chr = one_char_lookup.find(chr);
		if(find_chr != one_char_lookup.end()){
			result.emplace_back(find_chr->second, position, chr);
			position++;
			continue;
		}

		bool found_keyword = false;
		for(const auto &entry : keyword_lookup){
			const size_t key_length = entry.first.length();
			if(remaining >= key_length){
				const std::string substr = text.substr(position, key_length);
				if(substr == entry.first){
					result.emplace_back(entry.second, position, substr);
					position += key_length;
					found_keyword = true;
					break;
				}
			}
		}
		if(found_keyword){
			continue;
		}

		if(chr == '"'){
			bool terminated = false;
			std::string str = "";
			size_t str_end = position + 1;
			bool is_escape = false;
			while(!terminated && str_end < text.length()){
				const auto &str_chr = text.at(str_end);
				if(is_escape){
					if(str_chr == '"'){
						str.push_back('"');
					}else if(str_chr == 'n'){
						str.push_back('\n');
					}else if(str_chr == '\\'){
						str.push_back('\\');
					}else{
						// invalid escaping, ignore
						str.push_back('\\');
						str.push_back(str_chr);
					}
					is_escape = false;
				}else if(str_chr == '\\'){
					is_escape = true;
				}else if(str_chr == '"'){
					result.emplace_back(token_type_e::STRING, position, str);
					terminated = true;
				}else{
					str.push_back(str_chr);
				}
				str_end++;
			}
			if(!terminated){
				throw std::runtime_error("Unterminated string literal");
			}
			position = str_end + 1;
			continue;
		}

		if((chr >= '0' && chr <= '9') || chr == '-' || chr == '+'){
			bool found_dot = false;
			bool found_chrono = false;
			bool found_hex = false;
			bool found_bin = false;
			bool found_oct = false;
			bool is_terminated = (chr != '-' && chr != '+');
			size_t num_end = position + 1;
			while(num_end < text.length()){
				const auto &num_chr = text.at(num_end);
				if(is_whitespace(num_chr)){
					break;
				}else if(num_chr >= '0' && (num_chr <= '1' || (!found_bin && num_chr <= '7') || (!found_oct && num_chr <= '9'))){
					is_terminated = !found_chrono;
					num_end++;
				}else if(num_chr >= 'a' && num_chr <= 'f' && found_hex){
					is_terminated = !found_chrono;
					num_end++;
				}else if(num_chr == '.' && !found_dot && !found_chrono && !found_hex && !found_bin && !found_oct){
					found_dot = true;
					//is_terminated = false;  // allowed in C++
					num_end++;
				}else if(
					(num_chr == 'h' || num_chr == 'm' || num_chr == 's' || (num_chr == 'u' && num_end+1 < text.length() && text.at(num_end+1) == 's') || (num_chr == 'n' && num_end+1 < text.length() && text.at(num_end+1) == 's')) &&
					!found_dot && !found_hex && !found_bin && !found_oct
				){
					if(num_chr == 'u' || num_chr == 'n' || (num_chr == 'm' && num_end < text.length() && text.at(num_end) == 's')){
						num_end++;
					}
					found_chrono = true;
					is_terminated = true;
					num_end++;
				}else if(num_chr == 'x' && num_end == position+1 && text.at(position) == '0'){
					found_hex = true;
					is_terminated = false;
					num_end++;
				}else if(num_chr == 'b' && num_end == position+1 && text.at(position) == '0'){
					found_bin = true;
					is_terminated = false;
					num_end++;
				}else if(num_chr == 'o' && num_end == position+1 && text.at(position) == '0'){
					found_oct = true;
					is_terminated = false;
					num_end++;
				}else{
					throw std::runtime_error("Invalid character in number literal");
				}
			}
			if(!is_terminated){
				throw std::runtime_error("Unterminated number literal");
			}
			const std::string num = text.substr(position, num_end-position);
			if(found_dot){
				result.emplace_back(token_type_e::FLOAT, position, num);
			}else if(found_chrono){
				result.emplace_back(token_type_e::DURATION, position, num);
			}else{
				result.emplace_back(token_type_e::INTEGER, position, num);
			}
			position = num_end;
			continue;
		}

		if(is_alphanumeric(chr)){
			size_t id_end = position + 1;
			while(id_end < text.length() && is_alphanumeric(text.at(id_end))){
				id_end++;
			}
			result.emplace_back(token_type_e::IDENTIFIER, position, text.substr(position, id_end-position));
			position = id_end;
			continue;
		}

		throw std::runtime_error("Invalid character: " + std::string({chr}));
	}

	return result;
}


}
}
