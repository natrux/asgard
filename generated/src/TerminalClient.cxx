#include <asgard/run/TerminalClient.hxx>
#include <asgard/run/Terminal_read_char.hxx>
#include <asgard/run/Terminal_read_char_return.hxx>
#include <asgard/run/Terminal_read_event.hxx>
#include <asgard/run/Terminal_read_event_return.hxx>
#include <asgard/data/Exception.hxx>
#include <asgard/core/exception.h>


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
	auto ret_ = request<Terminal_read_char>(c);
	if(auto ret = std::dynamic_pointer_cast<const Terminal_read_char_return>(ret_)){
		return;
	}else if(auto ex = std::dynamic_pointer_cast<const data::Exception>(ret_)){
		throw core::exception(ex);
	}
	throw std::runtime_error("Return of unexpected type");
}


std::future<void> TerminalClient::read_char_(const char &c){
	call<Terminal_read_char>(c);
	return pending_requests_read_char[last_request_id()].get_future();
}


void TerminalClient::read_event(const terminal_event_e &event){
	auto ret_ = request<Terminal_read_event>(event);
	if(auto ret = std::dynamic_pointer_cast<const Terminal_read_event_return>(ret_)){
		return;
	}else if(auto ex = std::dynamic_pointer_cast<const data::Exception>(ret_)){
		throw core::exception(ex);
	}
	throw std::runtime_error("Return of unexpected type");
}


std::future<void> TerminalClient::read_event_(const terminal_event_e &event){
	call<Terminal_read_event>(event);
	return pending_requests_read_event[last_request_id()].get_future();
}


void TerminalClient::on_return_received(std::shared_ptr<const data::Return> ret){
	Super::on_return_received(ret);
	return_dispatch_helper<Terminal_read_char_return>(ret, pending_requests_read_char);
	return_dispatch_helper<Terminal_read_event_return>(ret, pending_requests_read_event);
}


}
}

