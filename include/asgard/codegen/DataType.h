#pragma once

#include <asgard/codegen/ClassType.h>


namespace asgard{
namespace codegen{


class DataType : public ClassType{
public:
	DataType(const std::string &name_, const std::vector<std::string> &name_space_, const std::string &path_package_):
		ClassType(name_, name_space_, path_package_)
	{}

	bool has_default_initializer() const;
	std::string get_default_initializer() const;

	std::string get_path_declaration() const override;
	std::string get_path_generated_header() const;
	std::string get_path_generated_source() const;
	std::string get_include_path() const;

	void parse(const Namespace &root_namespace) override;
	void generate_code() const override;

private:
	std::unique_ptr<std::string> default_initializer;

	void generate_header() const;
	void generate_source() const;
};


}
}