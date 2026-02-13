#include <asgard/data/LogMessage.hxx>
#include <asgard/type/TypeReader.h>
#include <asgard/type/TypeWriter.h>
#include <asgard/core/TypeRegistry.h>


namespace asgard{
namespace data{


code::Signature LogMessage::static_signature(){
	auto sig = Super::static_signature();
	sig.parents.push_back(sig.name);
	sig.name = "asgard.data.LogMessage";
	sig.members["time"] = code::get_typecode<decltype(time)>();
	sig.members["level"] = code::get_typecode<decltype(level)>();
	sig.members["unit"] = code::get_typecode<decltype(unit)>();
	sig.members["message"] = code::get_typecode<decltype(message)>();
	return sig;
}


std::shared_ptr<LogMessage> LogMessage::create(){
	return std::make_shared<LogMessage>();
}


LogMessage::LogMessage(){
	set_signature(static_signature());
}


void LogMessage::read_member(type::TypeReader &reader, const std::string &name, const code::Typecode &type){
	if(name == "time"){
		reader.read_type(time, type);
	}else if(name == "level"){
		reader.read_type(level, type);
	}else if(name == "unit"){
		reader.read_type(unit, type);
	}else if(name == "message"){
		reader.read_type(message, type);
	}else{
		Super::read_member(reader, name, type);
	}
}


void LogMessage::write_member(type::TypeWriter &writer, const std::string &name) const{
	if(name == "time"){
		writer.write_value(time);
	}else if(name == "level"){
		writer.write_value(level);
	}else if(name == "unit"){
		writer.write_value(unit);
	}else if(name == "message"){
		writer.write_value(message);
	}else{
		Super::write_member(writer, name);
	}
}


static core::register_type_t<LogMessage> register_type;


}
}
