#pragma once

#include <cstdint>


namespace asgard{
namespace util{


template<size_t width>
using uints_t = typename std::enable_if<
	width >= 1 && width <= 8,
	typename std::conditional<
		width <= 1,
		uint8_t,
		typename std::conditional<
			width <= 2,
			uint16_t,
			typename std::conditional<
				width <= 4,
				uint32_t,
				uint64_t
			>::type
		>::type
	>::type
>::type;

template<class T>
using uintw_t = uints_t<sizeof(T)>;


}
}
