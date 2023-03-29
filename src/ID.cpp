#include <asgard/core/ID.h>
#include <crc/crc_64_xz.h>

#include <random>
#include <sstream>
#include <mutex>


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
	stream << std::hex << value;
	str_value = "0x" + stream.str();
}


void ID::set_random(){
	uint64_t new_value = 0;
	const uint64_t one = 1;
	for(size_t i=0; i<sizeof(new_value)*8; i++){
		if(get_random_bit()){
			new_value |= (one << i);
		}
	}
	set(new_value);
}


ID::operator uint64_t() const{
	return value;
}


std::string ID::str() const{
	return str_value;
}


bool ID::get_random_bit(){
	static std::random_device device;
	static std::mt19937 generator(device());
	static std::bernoulli_distribution dist(0.5);
	static std::mutex mutex_random;

	{
		std::lock_guard<std::mutex> lock(mutex_random);
		return dist(generator);
	}
}


}
}

