#pragma once

#include <asgard/io/InputSource.h>

#ifdef _WIN32
#include <winsock2.h>
#endif


namespace asgard{
namespace io{



class SocketInputSource : public InputSource{
#ifdef _WIN32
	using socket_t = SOCKET;
#else
	using socket_t = int;
#endif
public:
	SocketInputSource(socket_t socket);
	size_t read(char *data, size_t length) override;

private:
	socket_t m_socket;
};



}
}

