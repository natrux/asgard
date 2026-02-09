#include <asgard/io/FileOutputSource.h>

#include <stdexcept>
#include <cstring>


namespace asgard{
namespace io{


FileOutputSource::FileOutputSource(const std::string &path, bool append){
	stream = std::fopen(path.c_str(), append ? "ab" : "wb");
	if(!stream){
		throw std::runtime_error("fopen('" + path + "') failed with: " + std::string(strerror(errno)));
	}
}


FileOutputSource::~FileOutputSource(){
	std::fclose(stream);
}


size_t FileOutputSource::write(const void *data, size_t length){
	const auto result = std::fwrite(data, 1, length, stream);
	if(result == 0){
		// FIXME fwrite() is not required to set errno
		throw std::runtime_error("fwrite() failed with: " + std::string(strerror(errno)));
	}
	return result;
}


}
}

