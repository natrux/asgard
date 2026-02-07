#include <asgard/io/PosixOutputSource.h>
#include <stdexcept>
#include <cstring>
#include <unistd.h>


namespace asgard{
namespace io{


PosixOutputSource::PosixOutputSource(int fd):
	m_fd(fd)
{
}


size_t PosixOutputSource::write(const void *data, size_t length){
	const ssize_t result = ::write(m_fd, data, length);
	if(result == -1){
		throw std::runtime_error("write() failed with: " + std::string(strerror(errno)));
	}
	return static_cast<size_t>(result);
}


}
}

