#include <asgard/data/DataPacket.hxx>
#include <asgard/io/TypeReader.h>
#include <asgard/io/TypeWriter.h>


namespace asgard{
namespace data{


code::Signature DataPacket::static_signature(){
	auto sig = Super::static_signature();
	sig.name = "asgard.data.DataPacket";
	sig.members.push_back("time");
	sig.members.push_back("payload");
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


bool DataPacket::try_read_member(io::TypeReader &reader, const std::string &name){
	if(Super::try_read_member(reader, name)){
		return true;
	}
	if(name == "time"){
		reader.read_type(time);
		return true;
	}else if(name == "payload"){
		reader.read_type(payload);
		return true;
	}
	return false;
}


bool DataPacket::try_write_member(io::TypeWriter &writer, const std::string &name) const{
	if(Super::try_write_member(writer, name)){
		return true;
	}
	if(name == "time"){
		writer.write_type(time);
		return true;
	}else if(name == "payload"){
		writer.write_type(payload);
		return true;
	}
	return false;
}


}
}
