#pragma once

#include <asgard/codegen/Declaration.h>


namespace asgard{
namespace codegen{


class Method{
public:
	void to_string(std::ostream &stream) const;
	std::string to_string() const;

private:
	std::string name;
	std::vector<Declaration> input;
	std::vector<Declaration> output;
	bool is_const = false;
	bool is_override = false;
	bool is_pure_virtual = false;
};


}
}