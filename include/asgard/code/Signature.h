#pragma once

#include <asgard/code/Typecode.h>

#include <string>
#include <map>


namespace asgard{
namespace code{


class Signature{
public:
	std::string name;
	std::vector<std::string> parents;
	std::map<std::string, Typecode> members;

	uint64_t hash() const;
	std::vector<std::string> lookup_chain(bool include_this=true) const;
};


}
}
