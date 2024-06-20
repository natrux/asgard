#include <asgard/net/SocketEndpoint.h>

#include <stdexcept>
#include <cstring>

#ifdef _WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <unistd.h>
#include <poll.h>
#include <fcntl.h>
#endif


namespace asgard{
namespace net{


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
#ifdef _WIN32
		const char *optval = reinterpret_cast<const char*>(&value);
#else
		const void *optval = &value;
#endif
		if(setsockopt(fd, IPPROTO_IPV6, IPV6_V6ONLY, optval, sizeof(value)) < 0){
			throw std::runtime_error("setsockopt(IPV6_V6ONLY, "+ std::to_string(value) +") failed with: " + std::string(strerror(errno)));
		}
	}
	m_socket = fd;
}


void SocketEndpoint::bind(void *addr, socklen_t length) const{
	if(reuse_address){
		int value = 1;
#ifdef _WIN32
		const char *optval = reinterpret_cast<const char*>(&value);
#else
		const void *optval = &value;
#endif
		if(setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, optval, sizeof(value)) < 0) {
			throw std::runtime_error("setsockopt(SO_REUSEADDR, " + std::to_string(value) + ") failed with: " + std::string(strerror(errno)));
		}
	}
	if(::bind(m_socket, static_cast<sockaddr *>(addr), length) == -1){
		throw std::runtime_error("bind() failed with: " + std::string(strerror(errno)));
	}
}


void SocketEndpoint::connect(void *addr, socklen_t length, bool allow_eagain) const{
	const bool has_timeout = (connect_timeout > time::immediate());
	if(has_timeout){
		set_blocking(false);
	}
	try{
		if(::connect(m_socket, static_cast<sockaddr *>(addr), length) == -1){
			if(!has_timeout || (errno != EWOULDBLOCK && errno != EINPROGRESS && (!allow_eagain || errno != EAGAIN))){
				throw std::runtime_error("connect() failed with: " + std::string(strerror(errno)));
			}

			bool success = false;
			time::duration remaining = connect_timeout;
			const auto start_time = time::now();
			size_t num_wait = 0;
			while(!success && remaining > time::immediate()){
				num_wait++;
				const int wait_ret = wait_writable(remaining);
				if(wait_ret > 0){
					int error = 0;
					socklen_t len = sizeof(error);
#ifdef _WIN32
					char *optval = reinterpret_cast<char *>(&error);
#else
					void *optval = &error;
#endif
					if(getsockopt(m_socket, SOL_SOCKET, SO_ERROR, optval, &len) < 0){
						throw std::runtime_error("getsockopt(SOL_SOCKET, SO_ERROR) failed with: " + std::string(strerror(errno)));
					}

					if(error == 0){
						success = true;
					}else{
						throw std::runtime_error("connect() failed with: " + std::string(strerror(error)));
					}
				}else if(wait_ret < 0 && errno != EINTR){
					throw std::runtime_error(
#ifdef _WIN32
						"select()"
#else
						"poll()"
#endif
						" failed with: " + std::string(strerror(errno))
					);
				}

				remaining = connect_timeout - time::since(start_time);
			}
			if(!success){
				// timeout, interrupt connection attempt
				shutdown();
				throw std::runtime_error(
					"connect() timed out after " + std::to_string(num_wait) + " call" + (num_wait == 1 ? "" : "s") + " to "
#ifdef _WIN32
					"select()"
#else
					"poll()"
#endif
				);
			}
		}
	}catch(const std::runtime_error &/*err*/){
		if(has_timeout){
			set_blocking(true);
		}
		throw;
	}
	if(has_timeout){
		set_blocking(true);
	}
}


void SocketEndpoint::set_socket(socket_t socket, bool connected_){
	m_socket = socket;
	connected = connected_;
}


void SocketEndpoint::set_blocking(bool blocking) const{
#ifdef _WIN32
	u_long non_blocking_mode = (blocking ? 0 : 1);
	if(ioctlsocket(m_socket, FIONBIO, &non_blocking_mode) != 0){
		throw std::runtime_error("ioctlsocket(FIONBIO) failed with: " + std::string(strerror(errno)));
	}
#else
	const int flags_before = fcntl(m_socket, F_GETFL);
	if(flags_before == -1){
		throw std::runtime_error("fcntl(F_GETFL) failed with: " + std::string(strerror(errno)));
	}

	const int flags_after = (blocking ? flags_before & ~O_NONBLOCK : flags_before | O_NONBLOCK);
	if(fcntl(m_socket, F_SETFL, flags_after) == -1){
		throw std::runtime_error("fcntl(F_SETFL) failed with: " + std::string(strerror(errno)));
	}
#endif
}


int SocketEndpoint::wait_writable(const time::duration &duration) const{
	const int remaining_us = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
#ifdef _WIN32
	fd_set fdset;
	FD_ZERO(&fdset);
	FD_SET(m_socket, &fdset);
	timeval tv;
	tv.tv_sec = remaining_us / 1000000;
	tv.tv_usec = remaining_us - tv.tv_sec * 1000000 + 1;
	return select(m_socket + 1, NULL, &fdset, NULL, &tv);
#else
	pollfd fd;
	fd.fd = m_socket;
	// POLLIN is needed to detect shutdown()
	fd.events = POLLOUT | POLLIN;
	return poll(&fd, 1, remaining_us / 1000 + 1);
#endif
}



}
}
