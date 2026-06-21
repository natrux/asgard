#pragma once

#include <asgard/type/Typecode.h>
#include <asgard/type/EnumMap.h>


namespace asgard{
namespace data{


class Enum{
public:
	static type::Typecode typecode();
	virtual ~Enum() = default;

	virtual type::EnumMap enum_map() const = 0;
	virtual void from_int(const type::enum_t &v) = 0;
	virtual type::enum_t to_int() const = 0;
	void from_string(const std::string &str);
	std::string to_string() const;
};


}
}
