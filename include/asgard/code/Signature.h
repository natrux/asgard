#pragma once

#include <asgard/code/Typecode.h>

#include <string>
#include <map>


namespace asgard{
namespace code{


class Signature{
public:
	std::string name;
	std::map<std::string, Typecode> members;

	uint64_t hash() const;
};


}
}
