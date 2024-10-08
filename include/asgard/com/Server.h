#pragma once

#include <asgard/com/ServerModule.hxx>
#include <asgard/net/Endpoint.h>

#include <thread>


namespace asgard{
namespace com{


class Server : public ServerModule{
public:
	Server(const std::string &name_);
	Server(const std::string &name_, std::unique_ptr<net::Endpoint> endpoint);
	Server(const std::string &name_, const std::string &address);

protected:
	void init() override;
	void main() override;

	void init_endpoint(const std::string &address);
	void init_endpoint(std::unique_ptr<net::Endpoint> endpoint);

private:
	std::string m_address;
	std::unique_ptr<net::Endpoint> m_endpoint;
	void accept_loop();
	virtual void spawn_gateway(std::unique_ptr<net::Endpoint> endpoint) = 0;

	void error_wait() const;
};


}
}

