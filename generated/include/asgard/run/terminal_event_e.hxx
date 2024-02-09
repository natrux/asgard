#pragma once


namespace asgard{
namespace run{


class terminal_event_e{
private:
	enum class enum_e{
		ARROW_UP,
		ARROW_DOWN,
		ARROW_LEFT,
		ARROW_RIGHT,
		CR,
		LF,
		BACKSPACE,
		DEL,
		POS1,
		END,
		TAB,
		END_OF_FILE,
	};
	enum_e value = static_cast<enum_e>(-1);

public:
	constexpr static enum_e ARROW_UP = enum_e::ARROW_UP;
	constexpr static enum_e ARROW_DOWN = enum_e::ARROW_DOWN;
	constexpr static enum_e ARROW_LEFT = enum_e::ARROW_LEFT;
	constexpr static enum_e ARROW_RIGHT = enum_e::ARROW_RIGHT;
	constexpr static enum_e CR = enum_e::CR;
	constexpr static enum_e LF = enum_e::LF;
	constexpr static enum_e BACKSPACE = enum_e::BACKSPACE;
	constexpr static enum_e DEL = enum_e::DEL;
	constexpr static enum_e POS1 = enum_e::POS1;
	constexpr static enum_e END = enum_e::END;
	constexpr static enum_e TAB = enum_e::TAB;
	constexpr static enum_e END_OF_FILE = enum_e::END_OF_FILE;

	constexpr terminal_event_e() = default;
	constexpr terminal_event_e(const terminal_event_e &other) = default;
	constexpr terminal_event_e(const enum_e &v) : value(v){}
	terminal_event_e &operator=(const enum_e &v){ value = v; return *this; }
	terminal_event_e &operator=(const terminal_event_e &other){ value = other.value; return *this; }

	constexpr operator enum_e() const{ return value; }
};


}
}

