#pragma once

#include <asgard/codegen/Type.h>


namespace asgard{
namespace codegen{


class Method;
class DataType;


class ModuleType : public Type{
public:
	ModuleType(const std::string &name_, const std::vector<std::string> &name_space_, const std::string &path_package_):
		Type(name_, name_space_, path_package_)
	{}

	std::string get_path_declaration() const override;

	void parse(const Namespace &root_namespace) override;
	void generate_code() const override;
	void generate_module_header() const;
	void generate_module_source() const;
	void generate_client_header() const;
	void generate_client_source() const;
	void generate_method_headers() const;
	void generate_method_sources() const;

private:
	std::set<std::shared_ptr<Method>> methods;
	std::set<std::shared_ptr<DataType>> processes;
};


}
}
