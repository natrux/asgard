#pragma once


#include <asgard/mod/Client.h>
#include <asgard/run/terminal_event_e.hxx>


namespace asgard{
namespace run{

class TerminalClient : public mod::Client{
	using Super = mod::Client;
public:
	TerminalClient(const core::ID &destination);
	TerminalClient(const std::string &destination);
	TerminalClient(pipe::PipeIn &&destination);

	void read_char(const char &c);
	std::future<void> read_char_(const char &c);
	void read_event(const terminal_event_e &event);
	std::future<void> read_event_(const terminal_event_e &event);

private:
	std::map<size_t, std::promise<void>> pending_requests_read_char;
	std::map<size_t, std::promise<void>> pending_requests_read_event;

	void on_return_received(std::shared_ptr<const data::Return> ret) override;
};

}
}

