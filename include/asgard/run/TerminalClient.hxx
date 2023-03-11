#pragma once


#include <asgard/mod/Client.h>
#include <asgard/run/terminal_event_e.hxx>


namespace asgard{
namespace run{

class TerminalClient : public mod::Client{
public:
	TerminalClient(const core::ID &destination);
	TerminalClient(const std::string &destination);
	TerminalClient(pipe::PipeIn &&destination);

	void read_char(const char &c);
	void read_char_async(const char &c);
	void read_event(const terminal_event_e &event);
	void read_event_async(const terminal_event_e &event);
};

}
}

