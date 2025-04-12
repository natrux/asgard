#include <asgard/io/InputSource.h>

#include <stdexcept>
#include <cstdint>


namespace asgard{
namespace io{


void InputSource::read_all(void *data_, size_t length){
	uint8_t *data = reinterpret_cast<uint8_t *>(data_);
	size_t received = 0;
	while(received < length){
		const size_t step = read(data+received, length-received);
		if(step == 0){
			throw std::runtime_error("read(): EOF");
		}
		received += step;
	}
}


}
}
