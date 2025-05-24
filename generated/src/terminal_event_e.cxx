#include <asgard/run/terminal_event_e.hxx>


namespace asgard{
namespace run{


const std::map<terminal_event_e::enum_e, std::string> terminal_event_e::enum_to_string = {
	{ARROW_UP, "ARROW_UP"},
	{ARROW_DOWN, "ARROW_DOWN"},
	{ARROW_LEFT, "ARROW_LEFT"},
	{ARROW_RIGHT, "ARROW_RIGHT"},
	{CR, "CR"},
	{LF, "LF"},
	{BACKSPACE, "BACKSPACE"},
	{DEL, "DEL"},
	{POS1, "POS1"},
	{END, "END"},
	{TAB, "TAB"},
	{END_OF_FILE, "END_OF_FILE"},
};


const std::map<std::string, terminal_event_e::enum_e> terminal_event_e::string_to_enum = {
	{"ARROW_UP", ARROW_UP},
	{"ARROW_DOWN", ARROW_DOWN},
	{"ARROW_LEFT", ARROW_LEFT},
	{"ARROW_RIGHT", ARROW_RIGHT},
	{"CR", CR},
	{"LF", LF},
	{"BACKSPACE", BACKSPACE},
	{"DEL", DEL},
	{"POS1", POS1},
	{"END", END},
	{"TAB", TAB},
	{"END_OF_FILE", END_OF_FILE},
};


void terminal_event_e::from_string(const std::string &str){
	const auto find = string_to_enum.find(str);
	if(find != string_to_enum.end()){
		value = find->second;
	}
}


std::string terminal_event_e::to_string() const{
	const auto find = enum_to_string.find(value);
	if(find != enum_to_string.end()){
		return find->second;
	}
	return "";
}


}
}
