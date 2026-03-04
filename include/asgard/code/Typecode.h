#pragma once

#include <vector>
#include <cstdint>
#include <string>


namespace asgard{
namespace code{


using typecode_e = uint8_t;
using length_t = uint64_t;
using enum_t = uint32_t;

class Typecode{
public:
	enum : typecode_e {
		TYPE_NULL = 0x00,

		TYPE_BOOL = 0x01,
		TYPE_U8 = 0x02,
		TYPE_I8 = 0x03,
		TYPE_U16 = 0x04,
		TYPE_I16 = 0x05,
		TYPE_U32 = 0x06,
		TYPE_I32 = 0x07,
		TYPE_U64 = 0x08,
		TYPE_I64 = 0x09,
		TYPE_F32 = 0x0a,
		TYPE_F64 = 0x0b,

		TYPE_LIST = 0x20,                                         // std::vector, std::array, std::set, std::unordered_set
		TYPE_MAP = 0x21,                                          // std::map, std::unordered_map
		TYPE_PAIR = 0x22,
		TYPE_TUPLE = 0x23,

		TYPE_DURATION = 0x40,
		TYPE_STRING = 0x41,
		TYPE_OPTIONAL = 0x42,
		TYPE_POINTER = 0x43,

		TYPE_VALUE = 0x60,
		TYPE_ENUM = 0x61,
	};

	typecode_e code;
	std::vector<Typecode> sub_types;

	Typecode() = default;
	explicit Typecode(const typecode_e &code_): code(code_) {}
	explicit Typecode(const typecode_e &code_, const std::vector<Typecode> &sub_types_): code(code_), sub_types(sub_types_) {}
};


}
}
