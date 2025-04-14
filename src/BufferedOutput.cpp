#include <asgard/io/BufferedOutput.h>

#include <cstring>


namespace asgard{
namespace io{


BufferedOutput::BufferedOutput(std::unique_ptr<OutputSource> source):
	m_source(std::move(source))
{
}


BufferedOutput::~BufferedOutput(){
	try{
		flush();
	}catch(...){
		// no exceptions in destructors
	}
}


void BufferedOutput::write(const void *data, size_t length){
	if(!m_source){
		return;
	}
	if(length > BUFFER_THRESHOLD){
		flush();
		m_source->write_all(data, length);
	}else{
		const size_t free_space = BUFFER_SIZE - end;
		if(free_space < length){
			flush();
		}
		memcpy(m_buffer+end, data, length);
		end += length;
	}
}


void BufferedOutput::write(const std::vector<uint8_t> &data){
	write(data.data(), data.size());
}


void BufferedOutput::flush(){
	if(!m_source){
		return;
	}
	if(end > 0){
		m_source->write_all(m_buffer, end);
	}
	end = 0;
}


}
}
