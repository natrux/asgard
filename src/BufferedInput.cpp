#include <asgard/io/BufferedInput.h>
#include <stdexcept>
#include <cstring>


namespace asgard{
namespace io{


BufferedInput::BufferedInput(std::unique_ptr<InputSource> source):
	m_source(std::move(source))
{
}


void BufferedInput::read(void *data_, size_t length){
	const size_t available = end - start;
	if(length <= available){
		read_from_buffer(data_, length);
		return;
	}

	read_from_buffer(data_, available);
	reset_buffer();

	uint8_t *data = reinterpret_cast<uint8_t *>(data_);
	const size_t remaining = length - available;
	if(remaining > BUFFER_THRESHOLD){
		m_source->read_all(data+available, remaining);
	}else{
		// The data is not too big, so requesting more and buffering it might be worth it.
		size_t collected = 0;
		while(collected < remaining){
			const size_t read = fill_buffer();
			if(read == 0){
				throw std::underflow_error("EOF");
			}
			collected += read;
		}
		read_from_buffer(data+available, remaining);
	}
}


std::vector<uint8_t> BufferedInput::read(size_t length){
	std::vector<uint8_t> result;
	result.resize(length);
	read(result.data(), length);
	return result;
}


void BufferedInput::read_from_buffer(void *data, size_t length){
	memcpy(data, m_buffer+start, length);
	start += length;
}


size_t BufferedInput::fill_buffer(){
	const size_t free_space = BUFFER_SIZE - end;
	const size_t read = m_source->read(m_buffer+end, free_space);
	end += read;
	return read;
}


void BufferedInput::reset_buffer(){
	start = 0;
	end = 0;
}


}
}

