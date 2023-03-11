#include <asgard/com/Server.h>
#include <asgard/com/Gateway.h>
#include <asgard/make_unique.h>
#include <asgard/io/BufferedInput.h>
#include <asgard/run/Terminal.h>

#include <stdexcept>
#include <iostream>
#include <chrono>



class EchoGateway : public asgard::com::Gateway{
public:
	EchoGateway(const std::string &name_, std::unique_ptr<asgard::net::Endpoint> endpoint):
		Gateway(name_, std::move(endpoint))
	{
	}

private:
	void keep_reading(std::unique_ptr<asgard::io::InputSource> input_source) override{
		asgard::io::BufferedInput input(std::move(input_source));

		std::string current = "";
		while(true){
			const char next_char = input.read_next();

			if(next_char == '\n'){
				log(INFO) << current;
				current.clear();
			}else{
				current.push_back(next_char);
			}
		}
	}
};




class EchoServer : public asgard::com::Server{
public:
	EchoServer(const std::string &name_, std::unique_ptr<asgard::net::Endpoint> endpoint):
		Server(name_, std::move(endpoint))
	{
	}

	EchoServer(const std::string &name_, const std::string &address):
		Server(name_, address)
	{
	}

private:
	void spawn_gateway(std::unique_ptr<asgard::net::Endpoint> endpoint) override{
		log(INFO) << "Spawning gateway";
		const std::string gateway_name = get_name() + ".Gateway." + endpoint->get_name();
		asgard::mod::Module::start_module<EchoGateway>(gateway_name, std::move(endpoint));
	}
};





int main(int argc, char **argv){
	if(argc != 2){
		std::cerr << "Usage: " << argv[0] << " endpoint" << std::endl;
		return 1;
	}
	const std::string address = argv[1];


	asgard::mod::Module::start_module<asgard::run::Terminal>("Terminal");
	asgard::mod::Module::start_module<EchoServer>("Server", address);


	//std::this_thread::sleep_for(std::chrono::seconds(60));
	//asgard::mod::Module::shutdown_all();
	asgard::mod::Module::wait_for_shutdown();

	return 0;
}


