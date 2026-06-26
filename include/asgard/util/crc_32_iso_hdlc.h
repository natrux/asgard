#pragma once

#include <cstdint>
#include <cstddef>


namespace asgard{
namespace util{


class CRC_32_ISO_HDLC{
public:
	static uint32_t compute(const void *data, size_t length);

	void update(const void *data, size_t length);
	uint32_t value() const;

private:
	static const uint32_t table[256];
	uint32_t current = 0xffffffff;
};


}
}
