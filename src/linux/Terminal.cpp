#include <asgard/run/Terminal.h>
#include <asgard/run/Terminal_read_char.hxx>
#include <asgard/run/Terminal_read_event.hxx>
#include <asgard/run/TerminalClient.hxx>


namespace asgard{
namespace run{


void Terminal::read_loop(){
	TerminalClient terminal(make_pipe_in());
	while(node_should_run()) {
		const auto c = std::getchar();
		if(c == EOF){
			terminal.read_event(terminal_event_e::END_OF_FILE);
			break;
		}else if(c == 27){
			// ESC
			const auto c2 = std::getchar();
			if(c2 == '['){
				const auto c3 = std::getchar();
				if(c3 == 'A'){
					terminal.read_event_(terminal_event_e::ARROW_UP);
				}else if(c3 == 'B'){
					terminal.read_event_(terminal_event_e::ARROW_DOWN);
				}else if(c3 == 'C'){
					terminal.read_event_(terminal_event_e::ARROW_RIGHT);
				}else if(c3 == 'D'){
					terminal.read_event_(terminal_event_e::ARROW_LEFT);
				}else if(c3 == 'H'){
					terminal.read_event_(terminal_event_e::POS1);
				}else if(c3 == 'F'){
					terminal.read_event_(terminal_event_e::END);
				}else if(c3 == '3'){
					const auto c4 = std::getchar();
					if(c4 == '~'){
						terminal.read_event_(terminal_event_e::DEL);
					}
				}
			}
		}else if(c == '\r'){
			terminal.read_event_(terminal_event_e::CR);
		}else if(c == '\n'){
			terminal.read_event_(terminal_event_e::LF);
		}else if(c == 127 || c == 8){
			terminal.read_event_(terminal_event_e::BACKSPACE);
		}else if(c == '\t'){
			terminal.read_event_(terminal_event_e::TAB);
		}else{
			terminal.read_char_(c);
		}
	}
}


void Terminal::set_terminal_mode(){
	tcgetattr(0, &saved_attributes);

	termios tmp = saved_attributes;
	tmp.c_lflag &= ~static_cast<decltype(tmp.c_lflag)>(ICANON);
	tmp.c_lflag &= ~static_cast<decltype(tmp.c_lflag)>(ECHO);
	tmp.c_cc[VMIN] = 1;
	tmp.c_cc[VTIME] = 0;
	tcsetattr(0, TCSANOW, &tmp);
}


void Terminal::reset_terminal_mode(){
	tcsetattr(0, TCSANOW, &saved_attributes);
}


}
}

