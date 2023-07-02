#include <asgard/codegen/packages.h>


namespace asgard{
namespace codegen{


std::string namespace_to_path(const std::vector<std::string> &name_space, const std::string &separator){
	std::string result = "";
	for(size_t i=0; i<name_space.size(); i++){
		if(i > 0){
			result += separator;
		}
		result += name_space[i];
	}
	return result;
}


std::string namespace_to_path(const std::string &package_path, const package_content_e &content_type, const std::vector<std::string> &name_space){
	std::string result = package_path + "/" + dir_package;
	if(content_type == package_content_e::DATA){
		result += "/" + dir_data;
	}else if(content_type == package_content_e::MODULES){
		result += "/" + dir_modules;
	}
	result += "/" + namespace_to_path(name_space, "/");
	return result;
}


}
}
