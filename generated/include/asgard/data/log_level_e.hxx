#pragma once


namespace asgard{
namespace data{


class log_level_e{
private:
	enum class enum_e{
		DEBUG,
		INFO,
		WARN,
		ERROR,
	};
	enum_e value = static_cast<enum_e>(-1);

public:
	constexpr static enum_e DEBUG = enum_e::DEBUG;
	constexpr static enum_e INFO = enum_e::INFO;
	constexpr static enum_e WARN = enum_e::WARN;
	constexpr static enum_e ERROR = enum_e::ERROR;

	constexpr log_level_e() = default;
	constexpr log_level_e(const enum_e &v) : value(v){}
	log_level_e &operator=(const enum_e &v){ value = v; return *this; }

	constexpr operator enum_e() const{ return value; }
};


}
}

