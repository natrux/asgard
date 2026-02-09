#include <asgard/io/FileInputSource.h>

#include <stdexcept>
#include <cstring>


namespace asgard{
namespace io{


FileInputSource::FileInputSource(const std::string &path){
	stream = std::fopen(path.c_str(), "rb");
	if(!stream){
		throw std::runtime_error("fopen('" + path + "') failed with: " + std::string(strerror(errno)));
	}
}


FileInputSource::~FileInputSource(){
	std::fclose(stream);
}


size_t FileInputSource::read(void *data, size_t length){
	const auto result = std::fread(data, 1, length, stream);
	if(result == 0){
		if(std::feof(stream)){
			return 0;
		}else if(std::ferror(stream)){
			// FIXME fread() is not required to set errno
			throw std::runtime_error("fread() failed with: " + std::string(strerror(errno)));
		}
	}
	return result;
}


}
}
