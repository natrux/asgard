#include <asgard/util/string.h>


namespace asgard{
namespace util{


std::vector<std::string> string_split(const std::string &string, char separator, size_t pos, bool skip_empty){
	std::vector<std::string> result;
	size_t current_pos = pos;
	while(true){
		size_t next_match = string.find(separator, current_pos);
		if(next_match == std::string::npos){
			if(current_pos != std::string::npos){
				result.push_back(string.substr(current_pos));
			}
			break;
		}else{
			std::string token = string.substr(current_pos, next_match-current_pos);
			if(!token.empty() || !skip_empty){
				result.push_back(token);
			}
			current_pos = next_match + 1;
		}
	}
	return result;
}


std::string string_join(const std::vector<std::string> &list, const std::string &glue){
	std::string result = "";
	for(const auto &str : list){
		if(!result.empty()){
			result += glue;
		}
		result += str;
	}
	return result;
}


}
}

