#include <asgard/code/EnumMap.h>
#include <asgard/util/crc_64_xz.h>


namespace asgard{
namespace code{


enum_t EnumMap::find(const std::string &value) const{
	const auto find = enum_map.find(value);
	if(find != enum_map.end()){
		return find->second;
	}
	return 0;
}


std::string EnumMap::find(enum_t value) const{
	const auto find = reverse_enum_map.find(value);
	if(find != reverse_enum_map.end()){
		return find->second;
	}
	return "";
}


uint64_t EnumMap::hash() const{
	util::CRC_64_XZ crc;
	crc.update(name.c_str(), name.length());
	for(const auto &entry : enum_map){
		crc.update(entry.first.c_str(), entry.first.length());
	}
	return crc.value();
}


}
}
