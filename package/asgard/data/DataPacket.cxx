#include <asgard/data/DataPacket.hxx>
#include <asgard/type/TypeReader.h>
#include <asgard/type/TypeWriter.h>
#include <asgard/core/TypeRegistry.h>


namespace asgard{
namespace data{


code::Signature DataPacket::static_signature(){
	auto sig = Super::static_signature();
	sig.parents.push_back(sig.name);
	sig.name = "asgard.data.DataPacket";
	sig.members["time"] = code::get_typecode<decltype(time)>();
	sig.members["payload"] = code::get_typecode<decltype(payload)>();
	return sig;
}


std::shared_ptr<DataPacket> DataPacket::create(){
	return std::make_shared<DataPacket>();
}


DataPacket::DataPacket(){
	set_signature(static_signature());
}


bool DataPacket::operator==(const DataPacket &other) const{
	return
		time == other.time &&
		payload == other.payload &&
	true;
}


void DataPacket::read_member(type::TypeReader &reader, const std::string &name, const code::Typecode &type){
	if(name == "time"){
		reader.read_type(time, type);
	}else if(name == "payload"){
		reader.read_type(payload, type);
	}else{
		Super::read_member(reader, name, type);
	}
}


void DataPacket::write_member(type::TypeWriter &writer, const std::string &name) const{
	if(name == "time"){
		writer.write_value(time);
	}else if(name == "payload"){
		writer.write_value(payload);
	}else{
		Super::write_member(writer, name);
	}
}


static core::register_type_t<DataPacket> register_type;


}
}
