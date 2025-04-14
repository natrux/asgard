#pragma once

#include <asgard/io/InputSource.h>

#include <vector>
#include <cstdint>


namespace asgard{
namespace io{


class VectorInputSource : public InputSource{
public:
	VectorInputSource() = default;
	VectorInputSource(std::vector<uint8_t> &&input);
	VectorInputSource(const std::vector<uint8_t> &input);

	size_t read(void *data, size_t length) override;

private:
	std::vector<uint8_t> buffer;
	size_t pos = 0;
};


}
}
