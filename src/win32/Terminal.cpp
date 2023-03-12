#include <asgard/run/Terminal.h>
#include <asgard/run/Terminal_read_char.hxx>
#include <asgard/run/Terminal_read_event.hxx>
#include <asgard/run/TerminalClient.hxx>


namespace asgard{
namespace run{


void Terminal::read_loop(){
	TerminalClient terminal(make_pipe_in());
	while(node_should_run()){
		INPUT_RECORD input_record;
		DWORD dummy_var;
		ReadConsoleInput(console_input_handle, &input_record, 1, &dummy_var);
		if(input_record.EventType != KEY_EVENT || !input_record.Event.KeyEvent.bKeyDown){
			continue;
		}
		const char c = input_record.Event.KeyEvent.uChar.AsciiChar;
		if(c == 0){
			// use virtual key code
			const WORD vk = input_record.Event.KeyEvent.wVirtualKeyCode;
			if(vk == VK_UP){
				terminal.read_event_(terminal_event_e::ARROW_UP);
			}else if(vk == VK_DOWN){
				terminal.read_event_(terminal_event_e::ARROW_DOWN);
			}else if(vk == VK_LEFT){
				terminal.read_event_(terminal_event_e::ARROW_LEFT);
			}else if(vk == VK_RIGHT){
				terminal.read_event_(terminal_event_e::ARROW_RIGHT);
			}else if(vk == VK_HOME){
				terminal.read_event_(terminal_event_e::POS1);
			}else if(vk == VK_END){
				terminal.read_event_(terminal_event_e::END);
			}else if(vk == VK_DELETE){
				terminal.read_event_(terminal_event_e::DEL);
			}
		}else if(c == '\t'){
			terminal.read_event_(terminal_event_e::TAB);
		}else if(c == 127 || c == 8){
			terminal.read_event_(terminal_event_e::BACKSPACE);
		}else if(c == '\r'){
			terminal.read_event_(terminal_event_e::CR);
		}else if(c == '\n'){
			terminal.read_event_(terminal_event_e::LF);
		}else{
			terminal.read_char_(c);
		}
	}
}


void Terminal::set_terminal_mode(){
	GetConsoleMode(console_input_handle, &saved_attributes);
	SetConsoleMode(console_input_handle, saved_attributes & ~static_cast<typeof(saved_attributes)>(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT));
}


void Terminal::reset_terminal_mode(){
	SetConsoleMode(console_input_handle, saved_attributes);
}


void Terminal::write_prompt_winapi() const{
	CONSOLE_SCREEN_BUFFER_INFO screen_buffer;
	GetConsoleScreenBufferInfo(console_output_handle, &screen_buffer);
	// erase the current line
	{
		const COORD first_cell = {0, screen_buffer.dwCursorPosition.Y};
		if(screen_buffer.dwSize.X < 0){
			throw std::runtime_error("screen_buffer.dwSize.X < 0");
		}
		DWORD dummy_var;
		FillConsoleOutputCharacter(console_output_handle, ' ', static_cast<DWORD>(screen_buffer.dwSize.X), first_cell, &dummy_var);
	}
	// start of the line
	out_stream << "\r";
	// print prompt and part 1 of input
	out_stream << prompt << line.substr(0, cursor);
	// print part 2 of input
	out_stream << line.substr(cursor);
	// place the cursor
	COORD cursor_pos = screen_buffer.dwCursorPosition;
	cursor_pos.X = prompt.length() + cursor;
	SetConsoleCursorPosition(console_output_handle, cursor_pos);
}


void Terminal::write_log_message_winapi(std::shared_ptr<const data::LogMessage> message) const{
	CONSOLE_SCREEN_BUFFER_INFO screen_buffer;
	GetConsoleScreenBufferInfo(console_output_handle, &screen_buffer);
	auto attributes = screen_buffer.wAttributes;
	bool color_changed = false;

	if(message->level == data::log_level_e::DEBUG){
		attributes &= ~static_cast<unsigned int>(FOREGROUND_RED | FOREGROUND_BLUE);
		attributes |= FOREGROUND_GREEN;
		color_changed = true;
	}else if(message->level == data::log_level_e::WARN){
		attributes &= ~static_cast<unsigned int>(FOREGROUND_BLUE);
		attributes |= (FOREGROUND_RED | FOREGROUND_GREEN);
		color_changed = true;
	}else if(message->level == data::log_level_e::ERROR){
		attributes &= ~static_cast<unsigned int>(FOREGROUND_GREEN | FOREGROUND_BLUE);
		attributes |= FOREGROUND_RED;
		color_changed = true;
	}

	if(color_changed){
		SetConsoleTextAttribute(console_output_handle, attributes);
	}
	out_stream << message->format();
	if(color_changed){
		SetConsoleTextAttribute(console_output_handle, screen_buffer.wAttributes);
	}
	out_stream << std::endl;
}


}
}
