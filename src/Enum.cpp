#include <asgard/data/Enum.h>
#include <asgard/type/TypeReader.h>
#include <asgard/type/TypeWriter.h>

#include <stdexcept>


namespace asgard{
namespace data{


code::Typecode Enum::typecode(){
	return code::Typecode(code::Typecode::TYPE_ENUM);
}


void Enum::from_string(const std::string &str){
	const auto map = enum_map();
	const auto value = map.find(str);
	from_int(value);
}


std::string Enum::to_string() const{
	const auto map = enum_map();
	const auto value = to_int();
	return map.find(value);
}


}
}
