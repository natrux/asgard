#include <asgard/data/Value.h>
#include <asgard/data/Bin.h>
#include <asgard/type/TypeReader.h>
#include <asgard/type/TypeWriter.h>
#include <asgard/core/TypeRegistry.h>

#include <stdexcept>


namespace asgard{
namespace data{


code::Typecode Value::typecode(){
	return code::Typecode(code::Typecode::TYPE_VALUE);
}


code::Signature Value::static_signature(){
	code::Signature sig;
	sig.name = "asgard.data.Value";
	return sig;
}


std::shared_ptr<Value> Value::create(){
	return std::make_shared<Value>();
}


Value::Value(){
	set_signature(static_signature());
}


code::Signature Value::signature() const{
	return dynamic_signature;
}


void Value::set_signature(const code::Signature &signature){
	auto new_signature = signature;
	// add / overwrite members that are natively embedded
	for(const auto &entry : dynamic_signature.members){
		new_signature.members[entry.first] = entry.second;
	}
	dynamic_signature = new_signature;
}


void Value::read_member(type::TypeReader &reader, const std::string &name, const code::Typecode &type){
	auto &bin = unknown_members[name];
	reader.read_type(bin, type);
}


void Value::write_member(type::TypeWriter &writer, const std::string &name) const{
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
