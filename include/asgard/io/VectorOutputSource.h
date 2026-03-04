#pragma once

#include <asgard/io/OutputSource.h>

#include <vector>
#include <cstdint>


namespace asgard{
namespace io{


class VectorOutputSource : public OutputSource{
public:
	size_t write(const void *data, size_t length) override;

	std::vector<uint8_t> get();

private:
	std::vector<uint8_t> buffer;
};


}
}
