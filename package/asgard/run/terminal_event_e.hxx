#pragma once

#include <asgard/data/Enum.h>


namespace asgard{
namespace run{


class terminal_event_e : public data::Enum{
public:
	enum enum_e : type::enum_t{
		ARROW_UP = 0x32272593,
		ARROW_DOWN = 0xcbf05ef5,
		ARROW_LEFT = 0xad6829a6,
		ARROW_RIGHT = 0xf3afb5a5,
		CR = 0x1fe83ee1,
		LF = 0x82aaf653,
		BACKSPACE = 0xfb229c80,
		DEL = 0x7ab64af5,
		POS1 = 0x51c714ee,
		END = 0x965b713b,
		TAB = 0xe5440186,
		END_OF_FILE = 0x9dc3070,
	};

	static type::EnumMap static_enum_map();

	terminal_event_e() = default;
	terminal_event_e(const enum_e &v);
	terminal_event_e &operator=(const terminal_event_e &other) = default;
	terminal_event_e &operator=(const enum_e &v);

	type::EnumMap enum_map() const override;
	void from_int(const type::enum_t &v) override;
	type::enum_t to_int() const override;

	operator enum_e() const;

private:
	static const enum_e zero = static_cast<enum_e>(0);
	static const std::map<type::enum_t, std::string> _enum_map;
	enum_e value = zero;
};


}
}
