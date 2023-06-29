#pragma once

#include <asgard/codegen/ClassType.h>


namespace asgard{
namespace codegen{


class BuiltinDataType : public ClassType{
public:
	BuiltinDataType(const std::string &name_, const std::string &builtin_name, const std::string &include_path):
		ClassType(name, {}, "")
	{}
};


}
}
