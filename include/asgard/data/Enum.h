#pragma once

#include <asgard/data/Value.h>

#include <map>
#include <string>


namespace asgard{
namespace data{


class Enum : public Value{
public:
	void read_from(io::TypeReader &reader, io::typecode_e code);
	void write_to(io::TypeWriter &writer) const;

private:
	virtual void from_string(const std::string &str) = 0;
	virtual std::string to_string() const = 0;
};


}
}
