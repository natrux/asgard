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


std::string namespace_to_path(const std::string &package_path, const std::vector<std::string> &name_space){
	return package_path + "/" + dir_package + "/" + namespace_to_path(name_space, "/");
}


}
}
