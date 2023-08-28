#include <asgard/run/Terminal.h>
#include <asgard/run/Terminal_read_char.hxx>
#include <asgard/run/Terminal_read_event.hxx>

#include <asgard/util/string.h>

#include <thread>
#include <csignal>


namespace asgard{
namespace run{


Terminal::Terminal(const std::string &name_):
	TerminalModule(name_)
{
#ifdef _WIN32
	console_input_handle = GetStdHandle(STD_INPUT_HANDLE);
	console_output_handle = GetStdHandle(STD_OUTPUT_HANDLE);
#endif
}


void Terminal::init(){
	subscribe(input_log);
}


void Terminal::main(){
	set_terminal_mode();
	std::thread read_thread(&Terminal::read_loop, this);

	Super::main();

	pthread_kill(read_thread.native_handle(), SIGINT);
	if(read_thread.joinable()){
		read_thread.join();
	}
	reset_terminal_mode();
}


void Terminal::read_char_sync(const int8_t &character){
	if(state != terminal_state_e::INPUT){
		return;
	}

	line.insert(cursor, 1, character);
	cursor++;

	write_prompt();
}


void Terminal::read_event_sync(const terminal_event_e &event){
	if(event == terminal_event_e::END_OF_FILE){
		log(WARN) << "EOF";
	}

	if(state == terminal_state_e::INACTIVE){
		if(event == terminal_event_e::LF || event == terminal_event_e::CR){
			out_stream << std::endl;
			state = terminal_state_e::INPUT;
		}
	}else if(state == terminal_state_e::DISPLAY){
		if(event == terminal_event_e::LF || event == terminal_event_e::CR){
			out_stream << std::endl;
			state = terminal_state_e::INACTIVE;
		}
	}else if(state == terminal_state_e::INPUT){
		switch(event){
		case terminal_event_e::ARROW_UP:
			// TODO
			break;
		case terminal_event_e::ARROW_DOWN:
			// TODO
			break;
		case terminal_event_e::ARROW_LEFT:
			if(cursor > 0){
				cursor--;
			}
			break;
		case terminal_event_e::ARROW_RIGHT:
			if(cursor < line.length()){
				cursor++;
			}
			break;
		case terminal_event_e::BACKSPACE:
			if(cursor > 0){
				cursor--;
				line.erase(cursor, 1);
			}
			break;
		case terminal_event_e::DEL:
			if(cursor < line.length()){
				line.erase(cursor, 1);
			}
			break;
		case terminal_event_e::POS1:
			cursor = 0;
			break;
		case terminal_event_e::END:
			cursor = line.length();
			break;
		case terminal_event_e::CR:
		case terminal_event_e::LF:
			out_stream << std::endl;
			state = execute(line);
			line = "";
			cursor = 0;
			break;
		case terminal_event_e::TAB:
			// TODO
			break;
		default:
			break;
		}
	}

	if(state == terminal_state_e::INPUT){
		write_prompt();
	}
}


void Terminal::process(std::shared_ptr<const data::LogMessage> value){
	if(state == terminal_state_e::INACTIVE){
		write_log_message(value);
	}
}


Terminal::terminal_state_e Terminal::execute(const std::string &command_line){
	size_t first_space = command_line.find(' ');
	std::string command = command_line.substr(0, first_space);
	std::vector<std::string> args = util::string_split(command_line, ' ', first_space, true);
	return execute(command, args);
}


Terminal::terminal_state_e Terminal::execute(const std::string &command, const std::vector<std::string> &args){
	if(command == "quit"){
		Module::shutdown_all();
		return terminal_state_e::INACTIVE;
	}else{
		if(!command.empty()){
			if(args.empty()){
				log(WARN) << "Unknown command: " << command;
			}else{
				log(WARN) << "Unknown command: " << command << "(" << util::string_join(args, ", ") << ")";
			}
		}
		return terminal_state_e::INACTIVE;
	}
}


void Terminal::write_prompt() const{
#ifdef _WIN32
	if(use_ansi_escape){
		write_prompt_ansi();
	}else{
		write_prompt_winapi();
	}
#else
	write_prompt_ansi();
#endif
}


void Terminal::write_prompt_ansi() const{
	// start of the line
	out_stream << "\r";
	// print prompt and part 1 of input
	out_stream << prompt << line.substr(0, cursor);
	// TODO: print suggestions
	// print part 2 of input
	out_stream << line.substr(cursor);
	// erase the rest of the line
	out_stream << "\x1b[0K";
	// place the cursor
	out_stream << "\r\x1b[" << (prompt.length()+cursor) << "C";
	out_stream.flush();
}


void Terminal::write_log_message(std::shared_ptr<const data::LogMessage> message) const{
#ifdef _WIN32
	if(use_ansi_escape){
		write_log_message_ansi(message);
	}else{
		write_log_message_winapi(message);
	}
#else
	write_log_message_ansi(message);
#endif
}


void Terminal::write_log_message_ansi(std::shared_ptr<const data::LogMessage> message) const{
	bool color_changed = false;
	if(message->level == data::log_level_e::DEBUG){
		out_stream << "\x1b[32m";
		color_changed = true;
	}else if(message->level == data::log_level_e::WARN){
		out_stream << "\x1b[33m";
		color_changed = true;
	}else if(message->level == data::log_level_e::ERROR){
		out_stream << "\x1b[31m";
		color_changed = true;
	}
	out_stream << message->format();
	if(color_changed){
		out_stream << "\x1b[39;49m";
	}
	out_stream << std::endl;
}


}
}

