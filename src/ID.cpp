#include <asgard/core/ID.h>
#include <asgard/core/random.h>
#include <crc/crc_64_xz.h>

#include <sstream>


namespace asgard{
namespace core{


ID::ID(){
	set_random();
}


ID::ID(const std::string &str){
	set(CRC_64_XZ::compute(str.c_str(), str.length()));
}


void ID::set(const uint64_t &new_value){
	value = new_value;

	std::stringstream stream;
	stream << "0x" << std::hex << value << std::dec;
	str_value = stream.str();
}


void ID::set_random(){
	set(rand64());
}


ID::operator uint64_t() const{
	return value;
}


std::string ID::str() const{
	return str_value;
}


}
}

