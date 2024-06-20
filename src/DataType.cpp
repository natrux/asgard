#include <asgard/codegen/DataType.h>
#include <asgard/codegen/Declaration.h>
#include <asgard/codegen/Method.h>
#include <asgard/codegen/packages.h>

#include <sstream>


namespace asgard{
namespace codegen{


bool DataType::has_default_initializer() const{
	return (default_initializer != nullptr);
}


void DataType::set_default_initializer(const std::string &init){
	default_initializer = std::make_unique<std::string>(init);
}


std::string DataType::get_default_initializer() const{
	if(!default_initializer){
		throw std::logic_error("Data type does not have a default initializer");
	}
	return *default_initializer;
}


std::string DataType::get_path_declaration() const{
	return path_package + "/" + dir_package + "/" + get_path_namespace("/") + "/" + name + ext_declaration_data;
}


std::string DataType::get_path_generated_header() const{
	return path_package + "/" + dir_generated_header + "/" + get_include_path();
}


std::string DataType::get_path_generated_source() const{
	return path_package + "/" + dir_generated_source + "/" + name + ext_generated_source;
}


std::string DataType::get_include_path() const{
	return get_path_namespace("/") + "/" + name + ext_generated_header;
}


void DataType::parse(const Namespace &/*root_namespace*/, const std::string &/*source*/){
	throw std::logic_error("Not implemented");
}


void DataType::generate_code() const{
	generate_header();
	generate_source();
}


void DataType::generate_header() const{
	// Test
	std::stringstream stream;

	stream << "#pragma once" << std::endl;
	stream << std::endl << std::endl;

	auto parent_data = std::dynamic_pointer_cast<ClassType>(parent);
	if(parent_data){
		stream << "#include <" << parent_data->get_include_path() << ">" << std::endl;
	}

	// TODO: other #includes

	for(const auto &space_name : name_space){
		stream << "namespace " << space_name << "{" << std::endl;
	}
	stream << std::endl << std::endl;


	stream << "class " << name;
	if(parent_data){
		stream << " : public " << parent_data->get_relative_name(name_space, "::");
	}
	stream << "{" << std::endl;

	stream << "public:" << std::endl;

	for(const auto &field : fields){
		stream << "\t";
		field->to_string(stream);
		stream << ";" << std::endl;
	}
	stream << std::endl;
	for(const auto &method : methods){
		stream << "\t";
		method->to_string(stream);
		stream << ";" << std::endl;
	}

	stream << "};" << std::endl;

	stream << std::endl << std::endl;
	for(size_t i=0; i<name_space.size(); i++){
		stream << "}" << std::endl;
	}

	throw std::logic_error("Not implemented");
}


void DataType::generate_source() const{
	// Test
	std::stringstream stream;

	stream << "#include <" << get_include_path() << ">" << std::endl;


	for(const auto &space_name : name_space){
		stream << "namespace " << space_name << "{" << std::endl;
	}
	stream << std::endl << std::endl;

	stream << std::endl << std::endl;
	for(size_t i=0; i<name_space.size(); i++){
		stream << "}" << std::endl;
	}

	throw std::logic_error("Not implemented");
}


}
}

