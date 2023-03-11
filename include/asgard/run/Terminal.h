#pragma once

#include <asgard/mod/Module.h>
#include <asgard/topic/TopicPtr.h>
#include <asgard/core/ReturnMe.h>
#include <asgard/run/terminal_event_e.hxx>
#include <asgard/run/Terminal_read_char_return.hxx>
#include <asgard/run/Terminal_read_event_return.hxx>
#include <asgard/data/LogMessage.h>

#include <iostream>
#ifdef _WIN32
#include <windows.h>
#undef ERROR
#else
#include <termios.h>
#endif

namespace asgard{
namespace run{


class Terminal : public mod::Module{
	using Super = mod::Module;
public:
	Terminal(const std::string &name_);

protected:
	void init() override;
	void main() override;

	void process(std::shared_ptr<const data::Value> value) override;
	void process(std::shared_ptr<const data::Request> request) override;

	void read_char_sync(const char &character);
	void read_char_async(const char &character, core::ReturnMe<Terminal_read_char_return> &&return_me);
	void read_event_sync(const terminal_event_e &event);
	void read_event_async(const terminal_event_e &event, core::ReturnMe<Terminal_read_event_return> &&return_me);

private:
	enum class terminal_state_e{
		INACTIVE,
		INPUT,
		DISPLAY,
	};

	topic::TopicPtr input_log = "asgard.log";
#ifdef _WIN32
	DWORD saved_attributes;
	HANDLE console_input_handle;
	HANDLE console_output_handle;
	bool use_ansi_escape = true;
#else
	termios saved_attributes;
#endif
	std::ostream &out_stream = std::cout;
	terminal_state_e state = terminal_state_e::INACTIVE;
	const std::string prompt = "> ";
	std::string line = "";
	size_t cursor = 0;

	void process(std::shared_ptr<const data::LogMessage> value);
	terminal_state_e execute(const std::string &command_line);
	terminal_state_e execute(const std::string &command, const std::vector<std::string> &args);

	void read_loop();
	void set_terminal_mode();
	void reset_terminal_mode();
	void write_prompt() const;
	void write_prompt_ansi() const;
#ifdef _WIN32
	void write_prompt_winapi() const;
#endif
	void write_log_message(std::shared_ptr<const data::LogMessage> message) const;
	void write_log_message_ansi(std::shared_ptr<const data::LogMessage> message) const;
#ifdef _WIN32
	void write_log_message_winapi(std::shared_ptr<const data::LogMessage> message) const;
#endif
};



}
}
