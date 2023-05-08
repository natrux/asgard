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
	char *data = reinterpret_cast<char *>(data_);
	size_t available = end - start;
	if(length <= available){
		read_from_buffer(data, length);
		return;
	}

	read_from_buffer(data, available);
	reset_buffer();

	size_t written = available;
	size_t remaining = length - written;
	if(remaining <= BUFFER_THRESHOLD){
		// The data is not too big, so requesting more and buffering it might be worth it.
		available = 0;
		while(available < remaining){
			size_t read = fill_buffer();
			if(read == 0){
				throw std::underflow_error("EOF");
			}
			available += read;
		}
		read_from_buffer(data+written, remaining);
	}else{
		while(written < length){
			size_t read = m_source->read(data+written, length-written);
			if(read == 0){
				throw std::underflow_error("EOF");
			}
			written += read;
		}
	}
}


char BufferedInput::read_next(){
	char result;
	read(&result, 1);
	return result;
}


void BufferedInput::read_from_buffer(char *data, size_t length){
	memcpy(data, m_buffer+start, length);
	start += length;
}


size_t BufferedInput::fill_buffer(){
	size_t free_space = BUFFER_SIZE - end;
	size_t read = m_source->read(m_buffer+end, free_space);
	end += read;
	return read;
}


void BufferedInput::reset_buffer(){
	start = 0;
	end = 0;
}


}
}

