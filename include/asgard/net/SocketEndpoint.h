#pragma once

#include <asgard/net/Endpoint.h>
#include <asgard/io/SocketInputSource.h>
#include <asgard/io/SocketOutputSource.h>
#ifdef _WIN32
#include <ws2tcpip.h>
#else
#include <netdb.h>
#endif


namespace asgard{
namespace net{



class SocketEndpoint : public Endpoint{
public:
	SocketEndpoint();
	std::string get_name() const override;

	bool is_open() const override;
	bool is_connected() const override;
	bool can_expect() const override;

	void expect() const override;
	std::unique_ptr<Endpoint> accept() const override;
	void shutdown() const override;
	void close() override;

	std::unique_ptr<io::InputSource> input_source() const override;
	std::unique_ptr<io::OutputSource> output_source() const override;

protected:
#ifdef _WIN32
	using socklen_t = int;
	using socket_t = SOCKET;
#else
	using socket_t = int;
	const socket_t INVALID_SOCKET = -1;
#endif
	void open(int family);
	void bind(void *addr, socklen_t length) const;
	void connect(void *addr, socklen_t length) const;
	void set_socket(socket_t socket, bool connected);
	bool connected = false;

private:
	const int listen_queue_size = 5;
	const bool use_dual_stack = true;
	const bool reuse_address = true;
	socket_t m_socket = INVALID_SOCKET;
	virtual std::unique_ptr<SocketEndpoint> from_socket(socket_t socket, bool connected) const = 0;
	virtual void bind() const = 0;
	void listen(int queue) const;
};



}
}

