#include <asgard/codegen/Type.h>
#include <asgard/codegen/Declaration.h>
#include <asgard/codegen/Method.h>
#include <asgard/codegen/packages.h>

#include <stdexcept>
#include <fstream>


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


bool Type::get_builtin() const{
	return is_builtin;
}


void Type::set_builtin(){
	is_builtin = true;
}


void Type::parse(const Namespace &root_namespace){
	if(is_builtin){
		throw std::logic_error("Nothing to parse for builtin type");
	}
	const std::string path = get_path_declaration();

	std::string source;
	{
		std::ifstream stream(path);
		if(!stream){
			throw std::runtime_error("Failed to open '" + path + "'");
		}
		source = std::string(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());
	}
	parse(root_namespace, source);
}


std::string Type::get_path_namespace(const std::string &separator) const{
	return namespace_to_path(name_space, separator);
}


}
}

