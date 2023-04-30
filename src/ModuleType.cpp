#include <asgard/codegen/ModuleType.h>
#include <asgard/codegen/Method.h>
#include <asgard/codegen/DataType.h>
#include <asgard/codegen/packages.h>

#include <stdexcept>


namespace asgard{
namespace codegen{


std::string ModuleType::get_path_declaration() const{
	return path_package + "/" + dir_package + "/" + dir_modules + "/" + get_path_namespace("/") + "/" + name + ext_declaration;
}


void ModuleType::parse(const Namespace &/*root_namespace*/){
	throw std::logic_error("Not implemented");
}


void ModuleType::generate_code() const{
	generate_module_header();
	generate_module_source();
	generate_client_header();
	generate_client_source();
	generate_method_headers();
	generate_method_sources();
}


void ModuleType::generate_module_header() const{
	// TODO
	throw std::logic_error("Not implemented");
}


void ModuleType::generate_module_source() const{
	// TODO
	throw std::logic_error("Not implemented");
}


void ModuleType::generate_client_header() const{
	// TODO
	throw std::logic_error("Not implemented");
}


void ModuleType::generate_client_source() const{
	// TODO
	throw std::logic_error("Not implemented");
}


void ModuleType::generate_method_headers() const{
	// TODO
	throw std::logic_error("Not implemented");
}


void ModuleType::generate_method_sources() const{
	// TODO
	throw std::logic_error("Not implemented");
}


}
}
