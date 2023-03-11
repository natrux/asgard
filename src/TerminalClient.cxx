#include <asgard/run/TerminalClient.hxx>
#include <asgard/run/Terminal_read_char.hxx>
#include <asgard/run/Terminal_read_char_return.hxx>
#include <asgard/run/Terminal_read_event.hxx>
#include <asgard/run/Terminal_read_event_return.hxx>


namespace asgard{
namespace run{


TerminalClient::TerminalClient(const core::ID &destination_):
	Client(destination_)
{
}


TerminalClient::TerminalClient(const std::string &destination_):
	Client(destination_)
{
}


TerminalClient::TerminalClient(pipe::PipeIn &&destination_):
	Client(std::move(destination_))
{
}


void TerminalClient::read_char(const char &c){
	auto ret_ = request_sync<Terminal_read_char>(c);
	if(auto ret = std::dynamic_pointer_cast<const Terminal_read_char_return>(ret_)){
	}
}


void TerminalClient::read_char_async(const char &c){
	request_async<Terminal_read_char>(c);
}


void TerminalClient::read_event(const terminal_event_e &event){
	auto ret_ = request_sync<Terminal_read_event>(event);
	if(auto ret = std::dynamic_pointer_cast<const Terminal_read_event_return>(ret_)){
	}
}


void TerminalClient::read_event_async(const terminal_event_e &event){
	request_async<Terminal_read_event>(event);
}


}
}

