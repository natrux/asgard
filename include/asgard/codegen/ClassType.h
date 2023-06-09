#pragma once

#include <asgard/codegen/Type.h>


namespace asgard{
namespace codegen{


class Declaration;
class Method;


class ClassType : public Type{
public:
	ClassType(const std::string &name_, const std::vector<std::string> &name_space_, const std::string &path_package_):
		Type(name_, name_space_, path_package_)
	{}

	virtual std::string get_include_path() const = 0;
	void set_parent(std::shared_ptr<Type> new_parent);

protected:
	std::shared_ptr<Type> parent;
	std::set<std::unique_ptr<Declaration>> fields;
	std::set<std::unique_ptr<Method>> methods;
};


}
}
