#include <asgard/data/log_level_e.hxx>
#include <asgard/io/TypeReader.h>
#include <asgard/io/TypeWriter.h>


namespace asgard{
namespace data{


const std::map<code::enum_t, std::string> log_level_e::_enum_map = {
	{DEBUG, "DEBUG"},
	{INFO, "INFO"},
	{WARN, "WARN"},
	{ERROR, "ERROR"},
};


code::EnumMap log_level_e::static_enum_map(){
	code::EnumMap map;
	map.name = "asgard.data.log_level_e";
	map.enum_map = _enum_map;
	map.fill_reverse();
	return map;
}


log_level_e::log_level_e(const enum_e &v):
	value(v)
{
}


log_level_e &log_level_e::operator=(const enum_e &v){
	value = v;
	return *this;
}


code::EnumMap log_level_e::enum_map() const{
	return static_enum_map();
}


void log_level_e::from_int(const code::enum_t &v){
	const auto find = _enum_map.find(v);
	if(find == _enum_map.end()){
		value = zero;
	}else{
		value = static_cast<enum_e>(v);
	}
}


code::enum_t log_level_e::to_int() const{
	return value;
}


log_level_e::operator enum_e() const{
	return value;
}


}
}
