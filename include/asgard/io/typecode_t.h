#pragma once

#include <vector>
#include <cstdint>


namespace asgard{
namespace io{


using typecode_e = uint8_t;
using length_t = uint64_t;

struct typecode_t{
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
		TYPE_DURATION = 0x0c,
		TYPE_STRING = 0x0d,
		TYPE_LIST = 0x0e,                                         // std::vector, std::array, std::set, std::unordered_set
		TYPE_MAP = 0x0f,                                          // std::map, std::unordered_map
		TYPE_PAIR = 0x10,
		TYPE_TUPLE = 0x11,
		TYPE_OPTIONAL = 0x12,
		TYPE_POINTER = 0x13,

		TYPE_VALUE = 0x80,
		TYPE_ENUM = 0x81,
	};

	typecode_e code;
	std::vector<typecode_t> sub_types;

	typecode_t() = default;
	typecode_t(const typecode_e &code_, const std::vector<typecode_t> &sub_types_={}): code(code_), sub_types(sub_types_) {}
};


}
}
