#pragma once

#include <asgard/com/Gateway.h>
#include <asgard/net/Endpoint.h>
#include <asgard/mod/Module.h>

#include <set>
#include <thread>
#include <memory>


namespace asgard{
namespace com{


class Server : public mod::Module{
public:
	Server(const std::string &name_, std::unique_ptr<net::Endpoint> endpoint);
	Server(const std::string &name_, const std::string &address);

protected:
	void main() override;

private:
	std::unique_ptr<net::Endpoint> m_endpoint;
	std::thread m_accept_thread;
	void accept_loop();
	virtual void spawn_gateway(std::unique_ptr<net::Endpoint> endpoint) = 0;
};


}
}

