#pragma once

#include <map>
#include <string>


namespace asgard{

namespace io{
class TypeReader;
class TypeWriter;
struct typecode_t;
}

namespace data{


class Enum{
public:
	virtual ~Enum() = default;

	virtual void from_string(const std::string &str) = 0;
	virtual std::string to_string() const = 0;
};


}
}
