#pragma once

#include <asgard/code/Typecode.h>
#include <asgard/code/EnumMap.h>


namespace asgard{
namespace data{


class Enum{
public:
	static code::Typecode typecode();
	virtual ~Enum() = default;

	virtual code::EnumMap enum_map() const = 0;
	virtual void from_int(const code::enum_t &v) = 0;
	virtual code::enum_t to_int() const = 0;
	void from_string(const std::string &str);
	std::string to_string() const;
};


}
}
