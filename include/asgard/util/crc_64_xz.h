#pragma once

#include <cstdint>
#include <cstddef>


namespace asgard{
namespace util{


class CRC_64_XZ{
public:
	static uint64_t compute(const void *data, size_t length);

	void update(const void *data, size_t length);
	uint64_t value() const;

private:
	static const uint64_t table[256];
	uint64_t current = 0xffffffffffffffff;
};


}
}

