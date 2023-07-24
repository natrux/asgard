#pragma once

#include <string>
#include <vector>


namespace asgard{
namespace codegen{


const std::string dir_package = "package";
const std::string dir_generated_header = "generated/include";
const std::string dir_generated_source = "generated/src";

const std::string ext_declaration_data = ".data";
const std::string ext_declaration_module = ".module";
const std::string ext_declaration_enum = ".enum";
const std::string ext_header = ".h";
const std::string ext_source = ".cpp";
const std::string ext_generated_header = ".hxx";
const std::string ext_generated_source = ".cxx";



std::string namespace_to_path(const std::vector<std::string> &name_space, const std::string &separator);
std::string namespace_to_path(const std::string &package_path, const std::vector<std::string> &name_space);


}
}
