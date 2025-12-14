#include <asgard/data/Value.h>
#include <asgard/io/TypeReader.h>
#include <asgard/io/TypeWriter.h>
#include <asgard/core/TypeRegistry.h>


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


code::Signature Value::signature() const{
	return static_signature();
}


void Value::read_member(io::TypeReader &reader, const std::string &name, const code::Typecode &code){
	if(!try_read_member(reader, name, code)){
		reader.skip(code);
	}
}


void Value::write_member(io::TypeWriter &writer, const std::string &name) const{
	if(!try_write_member(writer, name)){
		writer.write_value(nullptr);
	}
}


bool Value::try_read_member(io::TypeReader &/*reader*/, const std::string &/*name*/, const code::Typecode &/*code*/){
	return false;
}


bool Value::try_write_member(io::TypeWriter &/*writer*/, const std::string &/*name*/) const{
	return false;
}


static core::register_type_t<Value> register_type;


}
}
