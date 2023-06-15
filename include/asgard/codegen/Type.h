#pragma once

#include <asgard/codegen/Namespace.h>

#include <string>
#include <vector>
#include <set>
#include <memory>


namespace asgard{
namespace codegen{


class Type{
public:
	Type(const std::string &name_, const std::vector<std::string> &name_space_, const std::string &path_package_):
		name(name_),
		name_space(name_space_),
		path_package(path_package_)
	{}

	std::string get_name() const;
	std::string get_full_name(const std::string &separator) const;
	std::string get_relative_name(const std::vector<std::string> &other_namespace, const std::string &separator) const;
	virtual std::string get_path_declaration() const = 0;

	virtual void parse(const Namespace &root_namespace) = 0;
	virtual void generate_code() const = 0;

protected:
	std::string name;
	std::vector<std::string> name_space;
	std::string path_package;

	std::string get_path_namespace(const std::string &separator) const;
};


}
}
