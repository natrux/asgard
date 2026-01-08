#pragma once

#include <asgard/code/Typecode.h>

#include <map>
#include <string>


namespace asgard{
namespace data{


class Enum{
public:
	static code::Typecode typecode();
	virtual ~Enum() = default;

	virtual void from_string(const std::string &str) = 0;
	virtual std::string to_string() const = 0;
};


}
}
