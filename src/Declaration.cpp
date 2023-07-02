#include <asgard/codegen/Declaration.h>
#include <asgard/codegen/DataType.h>
#include <asgard/codegen/Method.h>

#include <sstream>


namespace asgard{
namespace codegen{


std::string Declaration::to_string() const{
	std::stringstream stream;
	if(is_const){
		stream << "const ";
	}
	stream << type->get_name();
	if(!template_types.empty()){
		std::vector<std::string> template_strings;
		for(auto t : template_types){
			template_strings.push_back(t->get_name());
		}
		stream << "<";
		for(size_t i=0; i<template_strings.size(); i++){
			if(i > 0){
				stream << ", ";
			}
			stream << template_strings[i];
		}
		stream << ">";
	}
	stream << " ";
	if(is_reference){
		stream << "&";
	}
	stream << name;
	if(initializer){
		stream << " = " << *initializer;
	}else if(type->has_default_initializer()){
		stream << " = " << type->get_default_initializer();
	}
	return stream.str();
}


}
}