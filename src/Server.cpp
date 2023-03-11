#include <asgard/com/Server.h>
#include <asgard/com/Gateway.h>
#include <asgard/make_unique.h>

#include <functional>
#include <stdexcept>
#include <cstring>

#ifndef _WIN32
#include <sys/socket.h>
#include <netdb.h>
#endif
#include <unistd.h>


namespace asgard{
namespace com{


Server::Server(const std::string &name_, std::unique_ptr<net::Endpoint> endpoint):
	Module(name_),
	m_endpoint(std::move(endpoint))
{
}


Server::Server(const std::string &name_, const std::string &address):
	Server(name_, net::Endpoint::from_address(address))
{
}

void Server::main(){
	m_endpoint->open();
	m_endpoint->expect();
	m_accept_thread = std::thread(&Server::accept_loop, this);

	Module::main();

	try{
		m_endpoint->shutdown();
	}catch(const std::exception &err){
		log(WARN) << err.what();
	}
	if(m_accept_thread.joinable()){
		m_accept_thread.join();
	}
	m_endpoint->close();
}


void Server::accept_loop(){
	while(node_should_run() && m_endpoint->is_open()){
		std::unique_ptr<net::Endpoint> connection;
		try{
			connection = m_endpoint->accept();
		}catch(const std::runtime_error &err){
			if(node_should_run()){
				// otherwise the exception is caused by shutdown() which is expected
				log(WARN) << err.what();
			}
		}
		if(connection){
			spawn_gateway(std::move(connection));
		}
	}
}


}
}

