#include <asgard/com/Server.h>


namespace asgard{
namespace com{


Server::Server(const std::string &name_):
	ServerModule(name_)
{
}


Server::Server(const std::string &name_, std::unique_ptr<net::Endpoint> endpoint):
	ServerModule(name_)
{
	init_endpoint(std::move(endpoint));
}


Server::Server(const std::string &name_, const std::string &address):
	Server(name_, net::Endpoint::from_address(address))
{
}


void Server::init(){
	if(!m_endpoint){
		throw std::logic_error("No endpoint given");
	}
}


void Server::main(){
	m_endpoint->open();
	m_endpoint->expect();
	m_accept_thread = std::thread(&Server::accept_loop, this);

	Super::main();

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


void Server::init_endpoint(std::unique_ptr<net::Endpoint> endpoint){
	if(m_endpoint){
		throw std::logic_error("Endpoint already given");
	}
	m_endpoint = std::move(endpoint);
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

