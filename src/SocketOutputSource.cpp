#include <asgard/io/SocketOutputSource.h>

#include <stdexcept>
#include <cstring>
#ifndef _WIN32
#include <sys/socket.h>
#endif


namespace asgard{
namespace io{


SocketOutputSource::SocketOutputSource(socket_t socket):
	m_socket(socket)
{
}


size_t SocketOutputSource::write(const void *data, size_t length){
	int flags = 0;
#ifdef _WIN32
	const char *data_ = reinterpret_cast<const char *>(data);
#else
	flags |= MSG_NOSIGNAL;
	const void *data_ = data;
#endif
	const ssize_t result = send(m_socket, data_, length, flags);
	if(result == -1){
		throw std::runtime_error("send() failed with: " + std::string(strerror(errno)));
	}
	return static_cast<size_t>(result);
}


}
}

