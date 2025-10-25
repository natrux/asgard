#include <asgard/data/Value.h>
#include <asgard/io/TypeReader.h>
#include <asgard/io/TypeWriter.h>


namespace asgard{
namespace data{


code::Signature Value::static_signature(){
	code::Signature sig;
	sig.name = "asgard.data.Value";
	return sig;
}


code::Signature Value::signature() const{
	return static_signature();
}


void Value::read_member(io::TypeReader &reader, const std::string &name){
	if(!try_read_member(reader, name)){
		reader.skip();
	}
}


void Value::write_member(io::TypeWriter &writer, const std::string &name) const{
	if(!try_write_member(writer, name)){
		writer.write_value(nullptr);
	}
}


bool Value::try_read_member(io::TypeReader &/*reader*/, const std::string &/*name*/){
	return false;
}


bool Value::try_write_member(io::TypeWriter &/*writer*/, const std::string &/*name*/) const{
	return false;
}


}
}
