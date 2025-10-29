#include <asgard/data/DataPacket.hxx>
#include <asgard/io/TypeReader.h>
#include <asgard/io/TypeWriter.h>


namespace asgard{
namespace data{


code::Signature DataPacket::static_signature(){
	auto sig = Super::static_signature();
	sig.name = "asgard.data.DataPacket";
	sig.members["time"] = code::get_typecode<decltype(time)>();
	sig.members["payload"] = code::get_typecode<decltype(payload)>();
	return sig;
}


code::Signature DataPacket::signature() const{
	return static_signature();
}


bool DataPacket::operator==(const DataPacket &other) const{
	return
		time == other.time &&
		payload == other.payload &&
	true;
}


bool DataPacket::try_read_member(io::TypeReader &reader, const std::string &name, const code::Typecode &code){
	if(Super::try_read_member(reader, name, code)){
		return true;
	}
	if(name == "time"){
		reader.read_type(time, code);
		return true;
	}else if(name == "payload"){
		reader.read_type(payload, code);
		return true;
	}
	return false;
}


bool DataPacket::try_write_member(io::TypeWriter &writer, const std::string &name) const{
	if(Super::try_write_member(writer, name)){
		return true;
	}
	if(name == "time"){
		writer.write_value(time);
		return true;
	}else if(name == "payload"){
		writer.write_value(payload);
		return true;
	}
	return false;
}


}
}
