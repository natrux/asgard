#include <asgard/net/TcpEndpoint.h>
#include <asgard/util/string.h>

#include <stdexcept>
#include <vector>
#include <cstring>


namespace asgard{
namespace net{


TcpEndpoint::TcpEndpoint():
	SocketEndpoint()
{
}


TcpEndpoint::TcpEndpoint(const std::string &host, unsigned int port):
	m_host(host),
	m_port(port)
{
}


bool TcpEndpoint::can_connect() const{
	return !m_host.empty();
}


std::string TcpEndpoint::get_host_service(const struct sockaddr *ai_addr, socklen_t ai_addrlen){
	char hostname[NI_MAXHOST];
	char servname[NI_MAXSERV];
	const auto flags = NI_NUMERICSERV;

	const auto error = getnameinfo(ai_addr, ai_addrlen, hostname, NI_MAXHOST, servname, NI_MAXSERV, flags);
	if(error != 0){
		throw std::runtime_error("getnameinfo() failed with: " + std::string(gai_strerror(error)));
	}

	return std::string(hostname) + ":" + std::string(servname);
}


void TcpEndpoint::bind() const{
	addrinfo *info = getaddrinfo();

	bool bind_success = false;
	std::vector<std::string> bind_errors;
	for(addrinfo *i=info; i!=NULL; i=i->ai_next){
		try{
			SocketEndpoint::bind(i->ai_addr, i->ai_addrlen);
			bind_success = true;
			break;
		}catch(const std::runtime_error &err){
			std::string name_service;
			try{
				name_service = get_host_service(i->ai_addr, i->ai_addrlen);
			}catch(const std::runtime_error &err_2){
				name_service = err_2.what();
			}
			bind_errors.push_back("[" + name_service + "]: " + err.what());
		}
	}
	freeaddrinfo(info);

	if(!bind_success){
		if(bind_errors.size() == 1){
			throw std::runtime_error(bind_errors.front());
		}else{
			const std::string summary = util::string_join(bind_errors, "; ");
			throw std::runtime_error("No suitable addrinfo found for bind(): " + summary);
		}
	}
}


void TcpEndpoint::connect(){
	addrinfo *info = getaddrinfo();

	bool connect_success = false;
	std::vector<std::string> connect_errors;
	for(addrinfo *i=info; i!=NULL; i=i->ai_next){
		try{
			connect_socket(i->ai_addr, i->ai_addrlen, false);
			connect_success = true;
			break;
		}catch(const std::runtime_error &err){
			std::string name_service;
			try{
				name_service = get_host_service(i->ai_addr, i->ai_addrlen);
			}catch(const std::runtime_error &err_2){
				name_service = err_2.what();
			}
			connect_errors.push_back("[" + name_service + "]: " + err.what());
		}
	}
	freeaddrinfo(info);

	if(!connect_success){
		if(connect_errors.size() == 1){
			throw std::runtime_error(connect_errors.front());
		}else{
			const std::string summary = util::string_join(connect_errors, "; ");
			throw std::runtime_error("No suitable addrinfo found for connect(): " + summary);
		}
	}
	connected = true;
}


addrinfo *TcpEndpoint::getaddrinfo(int family) const{
	std::string str_port = std::to_string(m_port);

	addrinfo hints;
	memset(&hints, 0, sizeof(addrinfo));
	hints.ai_family = family;
	hints.ai_socktype = SOCK_STREAM;

	addrinfo *info;
	int getaddrinfo_result = ::getaddrinfo(m_host.c_str(), str_port.c_str(), &hints, &info);
	if(getaddrinfo_result != 0){
		throw std::runtime_error("getaddrinfo() failed with: " + std::string(gai_strerror(getaddrinfo_result)));
	}

	return info;
}


Tcp4Endpoint::Tcp4Endpoint():
	TcpEndpoint()
{
}


Tcp4Endpoint::Tcp4Endpoint(const std::string &host, unsigned int port):
	TcpEndpoint(host, port)
{
}


void Tcp4Endpoint::open(){
	open_socket(AF_INET);
}


addrinfo *Tcp4Endpoint::getaddrinfo() const{
	return getaddrinfo(AF_INET);
}


std::unique_ptr<SocketEndpoint> Tcp4Endpoint::from_socket(socket_t socket, bool connected_) const{
	auto ret = std::make_unique<Tcp4Endpoint>();
	ret->set_socket(socket, connected_);
	return ret;
}


Tcp6Endpoint::Tcp6Endpoint():
	TcpEndpoint()
{
}


Tcp6Endpoint::Tcp6Endpoint(const std::string &host, unsigned int port):
	TcpEndpoint(host, port)
{
}


void Tcp6Endpoint::open(){
	open_socket(AF_INET6);
}


addrinfo *Tcp6Endpoint::getaddrinfo() const{
	return getaddrinfo(AF_INET6);
}


std::unique_ptr<SocketEndpoint> Tcp6Endpoint::from_socket(socket_t socket, bool connected_) const{
	auto ret = std::make_unique<Tcp6Endpoint>();
	ret->set_socket(socket, connected_);
	return ret;
}



}
}
