#pragma once

#include <string>
#include <vector>
#include <set>
#include <map>
#include <memory>


namespace asgard{
namespace codegen{


class Type;


class Namespace{
public:
	void insert(std::shared_ptr<Type> type);
	void insert(std::shared_ptr<Type> type, const std::vector<std::string> &name_space);
	std::shared_ptr<Type> find(const std::string &type_name) const;
	std::shared_ptr<Type> find(const std::string &type_name, const std::vector<std::string> &name_space) const;
	bool contains(const std::string &type_name) const;
	std::set<std::shared_ptr<Type>> get_types() const;

	Namespace &get(const std::string &name);
	Namespace &get(const std::vector<std::string> &names);

private:
	std::set<std::shared_ptr<Type>> types;
	std::map<std::string, Namespace> namespaces;
};


}
}
