#pragma once

#include <asgard/run/TerminalModule.hxx>

#include <iostream>
#ifdef _WIN32
#include <windows.h>
#undef ERROR
#else
#include <termios.h>
#endif

namespace asgard{
namespace run{


class Terminal : public TerminalModule{
public:
	Terminal(const std::string &name_);

protected:
	void main() override;

	void process(std::shared_ptr<const data::LogMessage> value) override;

	void read_char_sync(const int8_t &character) override;
	void read_event_sync(const terminal_event_e &event) override;

private:
	enum class terminal_state_e{
		INACTIVE,
		INPUT,
		DISPLAY,
	};

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
	std::string line = "";
	size_t cursor = 0;

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
