#include <asgard/io/FifoInputSource.h>

#include <cstring>
#include <stdexcept>


namespace asgard{
namespace io{


FifoInputSource::FifoInputSource(std::shared_ptr<util::Fifo> source_):
	source(source_)
{
	if(!source){
		throw std::logic_error("Invalid Fifo input source");
	}
}


size_t FifoInputSource::read(void *data, size_t size){
	const auto buffer = source->pop(size);
	std::memcpy(data, buffer.data(), buffer.size());
	return buffer.size();
}


}
}
