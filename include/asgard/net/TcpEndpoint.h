#pragma once

#include <asgard/net/SocketEndpoint.h>


namespace asgard{
namespace net{



class TcpEndpoint : public SocketEndpoint{
public:
	TcpEndpoint();
	TcpEndpoint(const std::string &host, unsigned int port);

	bool can_connect() const override;

	void connect() override;

protected:
	virtual addrinfo *getaddrinfo() const = 0;
	addrinfo *getaddrinfo(int family) const;

private:
	std::string m_host = "";
	unsigned int m_port = 0;
	static std::string get_host_service(const struct sockaddr *ai_addr, socklen_t ai_addrlen);
	void bind() const override;
};


class Tcp4Endpoint : public TcpEndpoint{
public:
	Tcp4Endpoint();
	Tcp4Endpoint(const std::string &host, unsigned int port);
	void open() override;

private:
	using TcpEndpoint::getaddrinfo;
	addrinfo *getaddrinfo() const override;
	std::unique_ptr<SocketEndpoint> from_socket(socket_t socket, bool connected) const override;
};


class Tcp6Endpoint : public TcpEndpoint{
public:
	Tcp6Endpoint();
	Tcp6Endpoint(const std::string &host, unsigned int port);
	void open() override;

private:
	using TcpEndpoint::getaddrinfo;
	addrinfo *getaddrinfo() const override;
	std::unique_ptr<SocketEndpoint> from_socket(socket_t socket, bool connected) const override;
};



}
}
