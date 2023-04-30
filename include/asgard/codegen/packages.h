#pragma once

#include <string>
#include <vector>


namespace asgard{
namespace codegen{


const std::string dir_package = "package";
const std::string dir_data = "data";
const std::string dir_modules = "modules";
const std::string dir_generated_header = "generated/include";
const std::string dir_generated_source = "generated/src";

const std::string ext_declaration = "";
const std::string ext_generated_header = ".hxx";
const std::string ext_generated_source = ".cxx";


enum class package_content_e{
	NONE,
	DATA,
	MODULES,
};


std::string namespace_to_path(const std::vector<std::string> &name_space, const std::string &separator);
std::string namespace_to_path(const std::string &package_path, const package_content_e &content_type, const std::vector<std::string> &name_space);


}
}
