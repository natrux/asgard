#include <asgard/io/VectorInputSource.h>

#include <cstring>


namespace asgard{
namespace io{


VectorInputSource::VectorInputSource(std::vector<uint8_t> &&input):
	buffer(std::move(input))
{
}


VectorInputSource::VectorInputSource(const std::vector<uint8_t> &input):
	buffer(input)
{
}


size_t VectorInputSource::read(void *data, size_t length){
	const size_t available = buffer.size() - pos;
	const size_t amount = std::min(length, available);
	if(amount > 0){
		memcpy(data, buffer.data(), amount);
		pos += amount;
	}
	return amount;
}


}
}
