#pragma once

#include <asgard/data/Enum.h>


namespace asgard{
namespace data{


class log_level_e : public Enum{
public:
	enum enum_e : type::enum_t{
		DEBUG = 0x9bf18517,
		INFO = 0xfd1cdce3,
		WARN = 0x1f2f5cb1,
		ERROR = 0xaa897ec1,
	};

	static type::EnumMap static_enum_map();

	log_level_e() = default;
	log_level_e(const enum_e &v);
	log_level_e &operator=(const log_level_e &other) = default;
	log_level_e &operator=(const enum_e &v);

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
