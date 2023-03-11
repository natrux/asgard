#pragma once

#include <asgard/util/crc.h>
#include <cstdint>


namespace asgard{
namespace util{


using CRC_64_XZ = CRC<64, 0x42f0e1eba9ea3693, true, true, 0xffffffffffffffff, 0xffffffffffffffff>;

template<>
const uint_w<64> CRC_64_XZ::table[256];


}
}
