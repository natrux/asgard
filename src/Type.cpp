#include <asgard/codegen/Type.h>
#include <asgard/codegen/Declaration.h>
#include <asgard/codegen/Method.h>
#include <asgard/codegen/packages.h>


namespace asgard{
namespace codegen{


std::string Type::get_name() const{
	return name;
}


std::string Type::get_full_name(const std::string &separator) const{
	return get_relative_name({}, separator);
}


std::string Type::get_relative_name(const std::vector<std::string> &other_namespace, const std::string &separator) const{
	auto my_iter = name_space.begin();
	auto their_iter = other_namespace.begin();
	while(my_iter != name_space.end() && their_iter != other_namespace.end()){
		if(*my_iter != *their_iter){
			break;
		}
	}

	std::vector<std::string> diff_namespace;
	diff_namespace.insert(diff_namespace.end(), my_iter, name_space.end());

	const auto ns_to_path = namespace_to_path(diff_namespace, separator);
	return (ns_to_path.empty() ? "" : ns_to_path + separator) + get_name();
}


std::string Type::get_path_namespace(const std::string &separator) const{
	return namespace_to_path(name_space, separator);
}


}
}

