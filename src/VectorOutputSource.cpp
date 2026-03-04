#include <asgard/io/VectorOutputSource.h>


namespace asgard{
namespace io{


size_t VectorOutputSource::write(const void *data_, size_t length){
	const uint8_t *data = reinterpret_cast<const uint8_t *>(data_);
	buffer.insert(buffer.end(), data, data+length);
	return length;
}


std::vector<uint8_t> VectorOutputSource::get(){
	std::vector<uint8_t> result = std::move(buffer);
	buffer.clear();
	return result;
}


}
}
