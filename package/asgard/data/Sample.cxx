#include <asgard/data/Sample.hxx>
#include <asgard/type/TypeReader.h>
#include <asgard/type/TypeWriter.h>
#include <asgard/core/TypeRegistry.h>


namespace asgard{
namespace data{


code::Signature Sample::static_signature(){
	auto sig = Super::static_signature();
	sig.parents.push_back(sig.name);
	sig.name = "asgard.data.Sample";
	sig.members["time"] = code::get_typecode<decltype(time)>();
	sig.members["retained"] = code::get_typecode<decltype(retained)>();
	sig.members["data"] = code::get_typecode<decltype(data)>();
	return sig;
}


std::shared_ptr<Sample> Sample::create(){
	return std::make_shared<Sample>();
}


Sample::Sample(){
	set_signature(static_signature());
}


void Sample::read_member(type::TypeReader &reader, const std::string &name, const code::Typecode &type){
	if(name == "time"){
		reader.read_type(time, type);
	}else if(name == "retained"){
		reader.read_type(retained, type);
	}else if(name == "data"){
		reader.read_type(data, type);
	}else{
		Super::read_member(reader, name, type);
	}
}


void Sample::write_member(type::TypeWriter &writer, const std::string &name) const{
	if(name == "time"){
		writer.write_value(time);
	}else if(name == "retained"){
		writer.write_value(retained);
	}else if(name == "data"){
		writer.write_value(data);
	}else{
		Super::write_member(writer, name);
	}
}


void Sample::set_member(const std::string &name, const Bin &value){
	if(name == "time"){
		value.to(time);
	}else if(name == "retained"){
		value.to(retained);
	}else if(name == "data"){
		value.to(data);
	}else{
		Super::set_member(name, value);
	}
}


Bin Sample::get_member(const std::string &name) const{
	if(name == "time"){
		return time;
	}else if(name == "retained"){
		return retained;
	}else if(name == "data"){
		return data;
	}
	return Super::get_member(name);
}


static core::register_type_t<Sample> register_type;


}
}
