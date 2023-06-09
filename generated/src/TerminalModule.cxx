#include <asgard/run/TerminalModule.hxx>
#include <asgard/run/Terminal_read_char.hxx>
#include <asgard/run/Terminal_read_event.hxx>


namespace asgard{
namespace run{


TerminalModule::TerminalModule(const std::string &name_):
	Super(name_)
{
}


void TerminalModule::read_char_sync(const char &/*character*/){
	throw std::logic_error("Not implemented");
}


void TerminalModule::read_char_async(const char &character, core::ReturnMe<Terminal_read_char_return> &&return_me){
	try{
		read_char_sync(character);
		return_me.retrn();
	}catch(const std::exception &err){
		return_me.err(err);
	}
}


void TerminalModule::read_event_sync(const terminal_event_e &/*event*/){
	throw std::logic_error("Not implemented");
}


void TerminalModule::read_event_async(const terminal_event_e &event, core::ReturnMe<Terminal_read_event_return> &&return_me){
	try{
		read_event_sync(event);
		return_me.retrn();
	}catch(const std::exception &err){
		return_me.err(err);
	}
}


void TerminalModule::process(std::shared_ptr<const data::Request> request){
	if(auto read_char_r = std::dynamic_pointer_cast<const Terminal_read_char>(request)){
		read_char_async(read_char_r->character, core::ReturnMe<Terminal_read_char_return>(request));
	}else if(auto read_event_r = std::dynamic_pointer_cast<const Terminal_read_event>(request)){
		read_event_async(read_event_r->event, core::ReturnMe<Terminal_read_event_return>(request));
	}else{
		Super::process(request);
	}
}


void TerminalModule::process(std::shared_ptr<const data::Sample> sample){
	auto data = sample->data;
	if(auto d = std::dynamic_pointer_cast<const data::LogMessage>(data)){
		process(sample, d);
	}else{
		Super::process(sample);
	}
}


void TerminalModule::process(std::shared_ptr<const data::Sample> /*sample*/, std::shared_ptr<const data::LogMessage> data){
	process(data);
}


void TerminalModule::process(std::shared_ptr<const data::LogMessage> /*data*/){
	throw std::logic_error("Not implemented");
}




}
}
