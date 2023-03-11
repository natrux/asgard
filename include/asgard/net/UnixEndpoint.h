#pragma once

#include <asgard/net/SocketEndpoint.h>


namespace asgard{
namespace net{



class UnixEndpoint : public SocketEndpoint{
public:
	UnixEndpoint();
	UnixEndpoint(const std::string &path);

	bool can_connect() const override;

	void open() override;
	void bind() const override;
	void connect() override;

private:
	std::string m_path = "";
	std::unique_ptr<SocketEndpoint> from_socket(int socket, bool connected) const override;
};



}
}
