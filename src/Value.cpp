#include <asgard/data/Value.h>
#include <asgard/data/Bin.h>
#include <asgard/io/TypeReader.h>
#include <asgard/io/TypeWriter.h>
#include <asgard/core/TypeRegistry.h>

#include <stdexcept>


namespace asgard{
namespace data{


code::Signature Value::static_signature(){
	code::Signature sig;
	sig.name = "asgard.data.Value";
	return sig;
}


std::shared_ptr<Value> Value::create(){
	return std::make_shared<Value>();
}


std::shared_ptr<Value> Value::create_with(const code::Signature &signature){
	auto result = std::make_shared<Value>();
	result->unknown_signature = signature;
	return result;
}


code::Signature Value::signature() const{
	if(!unknown_signature.name.empty()){
		return unknown_signature;
	}
	return static_signature();
}


void Value::read_member(io::TypeReader &reader, const std::string &name, const code::Typecode &code){
	auto &bin = unknown_members[name];
	reader.read_type(bin, code);
}


void Value::write_member(io::TypeWriter &writer, const std::string &name) const{
	const auto find = unknown_members.find(name);
	if(find != unknown_members.end()){
		writer.write_value(find->second);
		return;
	}
	throw std::logic_error("Unknown member: " + name);
}


static core::register_type_t<Value> register_type;


}
}
