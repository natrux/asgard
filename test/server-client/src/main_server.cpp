#include <EchoGatewayModule.hxx>
#include <EchoServerModule.hxx>

#include <asgard/io/BufferedInput.h>
#include <asgard/run/Terminal.h>

#include <iostream>


class EchoGateway : public EchoGatewayModule{
public:
	EchoGateway(const std::string &name_, std::unique_ptr<asgard::net::Endpoint> endpoint):
		EchoGatewayModule(name_)
	{
		init_endpoint(std::move(endpoint));
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




class EchoServer : public EchoServerModule{
public:
	EchoServer(const std::string &name_, std::unique_ptr<asgard::net::Endpoint> endpoint):
		EchoServerModule(name_)
	{
		init_endpoint(std::move(endpoint));
	}

	EchoServer(const std::string &name_, const std::string &address_):
		EchoServerModule(name_)
	{
		init_endpoint(address_);
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

	asgard::mod::Module::wait_for_shutdown();

	return 0;
}


