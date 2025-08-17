#pragma once

#include <asgard/data/Enum.h>
#include <asgard/io/get_typecode.h>


namespace asgard{
namespace data{


class log_level_e : public Enum{
private:
	enum class enum_e{
		DEBUG,
		INFO,
		WARN,
		ERROR,
	};
	enum_e value = static_cast<enum_e>(-1);

	static const std::map<enum_e, std::string> enum_to_string;
	static const std::map<std::string, enum_e> string_to_enum;

	void from_string(const std::string &str) override;
	std::string to_string() const override;

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


namespace asgard{
namespace io{


template<>
struct get_typecode_t<asgard::data::log_level_e>{
	static typecode_t value(){
		static typecode_t instance(typecode_t::TYPE_ENUM, "asgard.data.log_level_e");
		return instance;
	}
};


}
}
