#include <asgard/io/SocketInputSource.h>

#ifndef _WIN32
#include <sys/socket.h>
#endif
#include <stdexcept>
#include <cstring>


namespace asgard{
namespace io{


SocketInputSource::SocketInputSource(socket_t socket):
	m_socket(socket)
{
}


size_t SocketInputSource::read(void *data, size_t length){
#ifdef _WIN32
	char *data_ = reinterpret_cast<char *>(data);
#else
	void *data_ = data;
#endif
	const ssize_t result = recv(m_socket, data_, length, 0);
	if(result < 0){
		throw std::runtime_error("recv() failed with: " + std::string(strerror(errno)));
	}
	return static_cast<size_t>(result);
}


}
}

