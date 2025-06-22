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
	void read_from(io::TypeReader &reader, const io::typecode_t &type);
	void write_to(io::TypeWriter &writer) const;

private:
	virtual void from_string(const std::string &str) = 0;
	virtual std::string to_string() const = 0;
};


}
}
