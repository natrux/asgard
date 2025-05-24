#include <asgard/data/log_level_e.hxx>
#include <asgard/io/TypeReader.h>
#include <asgard/io/TypeWriter.h>


namespace asgard{
namespace data{


const std::map<log_level_e::enum_e, std::string> log_level_e::enum_to_string = {
	{DEBUG, "DEBUG"},
	{INFO, "INFO"},
	{WARN, "WARN"},
	{ERROR, "ERROR"},
};


const std::map<std::string, log_level_e::enum_e> log_level_e::string_to_enum = {
	{"DEBUG", DEBUG},
	{"INFO", INFO},
	{"WARN", WARN},
	{"ERROR", ERROR},
};


void log_level_e::from_string(const std::string &str){
	const auto find = string_to_enum.find(str);
	if(find != string_to_enum.end()){
		value = find->second;
	}
}


std::string log_level_e::to_string() const{
	const auto find = enum_to_string.find(value);
	if(find != enum_to_string.end()){
		return find->second;
	}
	return "";
}


}
}
