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
	ServerModule(name_)
{
	init_endpoint(address);
}


void Server::init(){
	if(!m_endpoint && !m_address.empty()){
		try{
			m_endpoint = net::Endpoint::from_address(m_address);
		}catch(const std::exception &err){
			throw std::runtime_error("Creating endpoint from '" + m_address + "' failed with: " + std::string(err.what()));
		}
	}
	if(!m_endpoint){
		throw std::logic_error("No endpoint given");
	}
	Super::init();
}


void Server::main(){
	m_endpoint->open();
	std::thread accept_thread(&Server::accept_loop, this);

	Super::main();

	try{
		m_endpoint->shutdown();
	}catch(const std::exception &err){
		log(WARN) << err.what();
	}
	if(accept_thread.joinable()){
		accept_thread.join();
	}
	m_endpoint->close();
}


void Server::init_endpoint(const std::string &address){
	if(m_endpoint || !m_address.empty()){
		throw std::logic_error("Endpoint already given");
	}
	m_address = address;
}


void Server::init_endpoint(std::unique_ptr<net::Endpoint> endpoint){
	if(m_endpoint || !m_address.empty()){
		throw std::logic_error("Endpoint already given");
	}
	m_endpoint = std::move(endpoint);
}


void Server::accept_loop(){
	bool bound = false;
	while(node_should_run() && !bound){
		try{
			m_endpoint->expect();
			bound = true;
		}catch(const std::exception &err){
			log(WARN) << err.what();
			error_wait();
		}
	}

	while(node_should_run() && m_endpoint->is_open()){
		std::unique_ptr<net::Endpoint> connection;
		try{
			connection = m_endpoint->accept();
		}catch(const std::exception &err){
			if(node_should_run()){
				// otherwise the exception is caused by shutdown() which is expected
				log(WARN) << err.what();
			}
		}
		if(connection){
			try{
				spawn_gateway(std::move(connection));
			}catch(const std::exception &err){
				log(WARN) << err.what();
			}
		}
	}
}


void Server::error_wait() const{
	const time::duration micro_time = 10ms;
	time::duration remaining = error_pause_time;
	while(node_should_run() && remaining > time::immediate()){
		const auto micro_wait = std::min(remaining, micro_time);
		std::this_thread::sleep_for(micro_wait);
		remaining -= micro_wait;
	}
}


}
}

