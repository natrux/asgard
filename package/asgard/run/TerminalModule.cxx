#include <asgard/run/TerminalModule.hxx>
#include <asgard/run/Terminal_read_char.hxx>
#include <asgard/run/Terminal_read_event.hxx>


namespace asgard{
namespace run{


TerminalModule::TerminalModule(const std::string &name_):
	Super(name_)
{
}


void TerminalModule::read_char_sync(const int8_t &/*character*/){
	throw std::logic_error("Not implemented");
}


void TerminalModule::read_char_async(const int8_t &character, core::ReturnMe<Terminal_read_char_return> &&return_me){
	try{
		read_char_sync(character);
		return_me.retrn();
	}catch(const std::exception &err){
		return_me.except(err);
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
		return_me.except(err);
	}
}


void TerminalModule::process(std::shared_ptr<const data::Request> request){
	if(auto read_char_r = std::dynamic_pointer_cast<const Terminal_read_char>(request)){
		core::ReturnMe<Terminal_read_char_return> return_me(request, make_pipe_in());
		add_pending_request(request, return_me.get_future());
		read_char_async(read_char_r->character, std::move(return_me));
	}else if(auto read_event_r = std::dynamic_pointer_cast<const Terminal_read_event>(request)){
		core::ReturnMe<Terminal_read_event_return> return_me(request, make_pipe_in());
		add_pending_request(request, return_me.get_future());
		read_event_async(read_event_r->event, std::move(return_me));
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
