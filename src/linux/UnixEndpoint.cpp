#include <asgard/net/UnixEndpoint.h>
#include <asgard/make_unique.h>
#include <stdexcept>
#include <unistd.h>
#include <sys/un.h>


namespace asgard{
namespace net{



inline sockaddr_un get_sockaddr(const std::string &path){
	sockaddr_un addr;
	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, path.c_str(), sizeof(addr.sun_path)-1);
	return addr;
}



UnixEndpoint::UnixEndpoint():
	SocketEndpoint()
{
}


UnixEndpoint::UnixEndpoint(const std::string &path):
	SocketEndpoint(),
	m_path(path)
{
}


bool UnixEndpoint::can_connect() const{
	return !m_path.empty();
}


void UnixEndpoint::open(){
	SocketEndpoint::open(AF_UNIX);
}


void UnixEndpoint::bind() const{
	unlink(m_path.c_str());
	sockaddr_un addr = get_sockaddr(m_path);
	SocketEndpoint::bind(&addr, sizeof(addr));
}


void UnixEndpoint::connect(){
	sockaddr_un addr = get_sockaddr(m_path);
	SocketEndpoint::connect(&addr, sizeof(addr), true);
	connected = true;
}


std::unique_ptr<SocketEndpoint> UnixEndpoint::from_socket(socket_t socket, bool connected_) const{
	auto ret = std::make_unique<UnixEndpoint>();
	ret->set_socket(socket, connected_);
	return ret;
}


}
}
