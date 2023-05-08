#pragma once

#include <asgard/io/OutputSource.h>

#ifdef _WIN32
#include <winsock2.h>
#endif

namespace asgard{
namespace io{


class SocketOutputSource : public OutputSource{
#ifdef _WIN32
	using socket_t = SOCKET;
#else
	using socket_t = int;
#endif
public:
	SocketOutputSource(socket_t socket);
	size_t write(const void *data, size_t length) override;

private:
	socket_t m_socket;
};



}
}

