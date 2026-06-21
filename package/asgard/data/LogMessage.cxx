#include <asgard/data/LogMessage.hxx>
#include <asgard/type/TypeReader.h>
#include <asgard/type/TypeWriter.h>
#include <asgard/core/TypeRegistry.h>


namespace asgard{
namespace data{


type::Signature LogMessage::static_signature(){
	auto sig = Super::static_signature();
	sig.parents.push_back(sig.name);
	sig.name = "asgard.data.LogMessage";
	sig.members["time"] = type::get_typecode<decltype(time)>();
	sig.members["wall_time"] = type::get_typecode<decltype(wall_time)>();
	sig.members["level"] = type::get_typecode<decltype(level)>();
	sig.members["unit"] = type::get_typecode<decltype(unit)>();
	sig.members["message"] = type::get_typecode<decltype(message)>();
	return sig;
}


std::shared_ptr<LogMessage> LogMessage::create(){
	return std::make_shared<LogMessage>();
}


LogMessage::LogMessage(){
	set_signature(static_signature());
}


void LogMessage::read_member(type::TypeReader &reader, const std::string &name, const type::Typecode &type){
	if(name == "time"){
		reader.read_type(time, type);
	}else if(name == "wall_time"){
		reader.read_type(wall_time, type);
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
	}else if(name == "wall_time"){
		writer.write_value(wall_time);
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


void LogMessage::set_member(const std::string &name, const Bin &value){
	if(name == "time"){
		value.to(time);
	}else if(name == "wall_time"){
		value.to(wall_time);
	}else if(name == "level"){
		value.to(level);
	}else if(name == "unit"){
		value.to(unit);
	}else if(name == "message"){
		value.to(message);
	}else{
		Super::set_member(name, value);
	}
}


Bin LogMessage::get_member(const std::string &name) const{
	if(name == "time"){
		return time;
	}else if(name == "wall_time"){
		return wall_time;
	}else if(name == "level"){
		return level;
	}else if(name == "unit"){
		return unit;
	}else if(name == "message"){
		return message;
	}
	return Super::get_member(name);
}


static core::register_type_t<LogMessage> register_type;


}
}
