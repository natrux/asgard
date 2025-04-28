#pragma once

#include <cstdint>


namespace asgard{
namespace io{


typedef uint8_t typecode_t;

enum : typecode_t {
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
	//TYPE_TUPLE = 0x11,

	//TYPE_ENUM = 0xfe,
	//TYPE_CLASS = 0xff,
};


}
}
