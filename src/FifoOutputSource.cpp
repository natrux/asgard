#include <asgard/io/FifoOutputSource.h>

#include <stdexcept>


namespace asgard{
namespace io{


FifoOutputSource::FifoOutputSource(std::shared_ptr<util::Fifo> source_):
	source(source_)
{
	if(!source){
		throw std::logic_error("Invalid Fifo output source");
	}
}


size_t FifoOutputSource::write(const void *data, size_t length){
	const uint8_t *data_ = reinterpret_cast<const uint8_t *>(data);
	source->push(data_, length);
	return length;
}


}
}
