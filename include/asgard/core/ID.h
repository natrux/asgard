#pragma once

#include <string>


namespace asgard{
namespace core{


class ID{
public:
	ID();
	ID(const std::string &str);
	ID(const ID &other) = default;
	ID(ID &&other) = default;
	ID &operator=(const ID &other) = default;
	ID &operator=(ID &&other) = default;

	void set(const uint64_t &new_value);
	void set_random();
	operator uint64_t() const;
	std::string str() const;

private:
	uint64_t value;
	std::string str_value;
};


}
}
