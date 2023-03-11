#include <asgard/io/OutputSource.h>
#include <stdexcept>


namespace asgard{
namespace io{


void OutputSource::write_all(const char *data, size_t length){
	size_t written = 0;
	while(written < length){
		size_t step = write(data+written, length-written);
		if(step == 0){
			throw std::runtime_error("write(): EOF");
		}
		written += step;
	}
}


void OutputSource::write_all(const std::string &data){
	write_all(data.c_str(), data.length());
}


}
}

