#include <asgard/net/SocketEndpoint.h>
#include <asgard/make_unique.h>

#include <stdexcept>
#include <cstring>

#ifdef _WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <unistd.h>
#endif


namespace asgard{
namespace net{


SocketEndpoint::SocketEndpoint(){
}


std::string SocketEndpoint::get_name() const{
	return std::to_string(m_socket);
}


bool SocketEndpoint::is_open() const{
	return (m_socket != INVALID_SOCKET);
}


bool SocketEndpoint::is_connected() const{
	return connected;
}


bool SocketEndpoint::can_expect() const{
	return is_open();
}


void SocketEndpoint::expect() const{
	bind();
	listen(listen_queue_size);
}


void SocketEndpoint::listen(int queue) const{
	if(::listen(m_socket, queue) != 0){
		throw std::runtime_error("listen() failed with: " + std::string(strerror(errno)));
	}
}


std::unique_ptr<Endpoint> SocketEndpoint::accept() const{
	socket_t socket = ::accept(m_socket, NULL, NULL);
	if(socket == INVALID_SOCKET){
		throw std::runtime_error("accept() failed with: " + std::string(strerror(errno)));
	}
	return from_socket(socket, true);
}


void SocketEndpoint::shutdown() const{
	if(m_socket == INVALID_SOCKET) return;
#ifdef _WIN32
	::shutdown(m_socket, SD_BOTH);
#else
	if(::shutdown(m_socket, SHUT_RDWR) == -1){
		throw std::runtime_error("shutdown() failed with: " + std::string(strerror(errno)));
	}
#endif
}


void SocketEndpoint::close(){
	if(m_socket == INVALID_SOCKET) return;
#ifdef _WIN32
	closesocket(m_socket);
#else
	::close(m_socket);
#endif
	m_socket = INVALID_SOCKET;
	connected = false;
}


std::unique_ptr<io::InputSource> SocketEndpoint::input_source() const{
	return std::make_unique<io::SocketInputSource>(m_socket);
}


std::unique_ptr<io::OutputSource> SocketEndpoint::output_source() const{
	return std::make_unique<io::SocketOutputSource>(m_socket);
}


void SocketEndpoint::open(int family){
	if(m_socket != INVALID_SOCKET){
		return;
	}

	socket_t fd = socket(family, SOCK_STREAM, 0);
	if(fd == INVALID_SOCKET){
		throw std::runtime_error("socket() failed with: " + std::string(strerror(errno)));
	}
	if(family == AF_INET6 && use_dual_stack){
		// Also open the IPv4 address (dual stack).
		// This only has an effect for INADDR_ANY, i.e. '::' which then also opens '0.0.0.0' on the same port.
		int value = 0;
		if(setsockopt(fd, IPPROTO_IPV6, IPV6_V6ONLY, reinterpret_cast<char*>(&value), sizeof(int)) < 0){
			throw std::runtime_error("setsockopt(IPV6_V6ONLY, "+ std::to_string(value) +") failed with: " + std::string(strerror(errno)));
		}
	}
	m_socket = fd;
}


void SocketEndpoint::bind(void *addr, socklen_t length) const{
	if(reuse_address){
		int value = 1;
		if(setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char*>(&value), sizeof(int)) < 0) {
			throw std::runtime_error("setsockopt(SO_REUSEADDR, " + std::to_string(value) + ") failed with: " + std::string(strerror(errno)));
		}
	}
	if(::bind(m_socket, static_cast<sockaddr *>(addr), length) == -1){
		throw std::runtime_error("bind() failed with: " + std::string(strerror(errno)));
	}
}


void SocketEndpoint::connect(void *addr, socklen_t length) const{
	if(::connect(m_socket, static_cast<sockaddr *>(addr), length) == -1){
		throw std::runtime_error("connect() failed with: " + std::string(strerror(errno)));
	}
}


void SocketEndpoint::set_socket(socket_t socket, bool connected_){
	m_socket = socket;
	connected = connected_;
}



}
}
