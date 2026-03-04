#include <asgard/io/PosixInputSource.h>
#include <unistd.h>
#include <stdexcept>
#include <cstring>


namespace asgard{
namespace io{


PosixInputSource::PosixInputSource(int fd):
	m_fd(fd)
{
}


size_t PosixInputSource::read(void *data, size_t length){
	const ssize_t result = ::read(m_fd, data, length);
	if(result < 0){
		throw std::runtime_error("read() failed with: " + std::string(strerror(errno)));
	}
	return static_cast<size_t>(result);
}


}
}
