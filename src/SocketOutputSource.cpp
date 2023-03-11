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


size_t SocketOutputSource::write(const char *data, size_t length){
	int flags = 0;
#ifndef _WIN32
	flags |= MSG_NOSIGNAL;
#endif
	ssize_t result = send(m_socket, data, length, flags);
	if(result == -1){
		throw std::runtime_error("send() failed with: " + std::string(strerror(errno)));
	}
	return static_cast<size_t>(result);
}


}
}

