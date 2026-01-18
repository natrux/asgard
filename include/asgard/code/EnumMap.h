#pragma once

#include <asgard/code/Typecode.h>

#include <string>
#include <map>


namespace asgard{
namespace code{


class EnumMap{
public:
	std::string name;
	std::map<std::string, enum_t> enum_map;
	std::map<enum_t, std::string> reverse_enum_map;

	enum_t find(const std::string &value) const;
	std::string find(enum_t value) const;
	uint64_t hash() const;
};


}
}
