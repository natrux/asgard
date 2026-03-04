#include <asgard/run/terminal_event_e.hxx>


namespace asgard{
namespace run{


const std::map<code::enum_t, std::string> terminal_event_e::_enum_map = {
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


code::EnumMap terminal_event_e::static_enum_map(){
	code::EnumMap map;
	map.name = "asgard.run.terminal_event_e";
	map.enum_map = _enum_map;
	map.fill_reverse();
	return map;
}


terminal_event_e::terminal_event_e(const enum_e &v):
	value(v)
{
}


terminal_event_e &terminal_event_e::operator=(const enum_e &v){
	value = v;
	return *this;
}


code::EnumMap terminal_event_e::enum_map() const{
	return static_enum_map();
}


void terminal_event_e::from_int(const code::enum_t &v){
	const auto find = _enum_map.find(v);
	if(find == _enum_map.end()){
		value = zero;
	}else{
		value = static_cast<enum_e>(v);
	}
}


code::enum_t terminal_event_e::to_int() const{
	return value;
}


terminal_event_e::operator enum_e() const{
	return value;
}


}
}
