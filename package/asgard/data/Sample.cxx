#include <asgard/data/Sample.hxx>
#include <asgard/io/TypeReader.h>
#include <asgard/io/TypeWriter.h>
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


void Sample::read_member(io::TypeReader &reader, const std::string &name, const code::Typecode &type){
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


void Sample::write_member(io::TypeWriter &writer, const std::string &name) const{
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


static core::register_type_t<Sample> register_type;


}
}
