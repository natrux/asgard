#include <asgard/com/Gateway.h>
#include <asgard/run/Terminal.h>

#include <StdIn.h>
#include <TextLine.h>

#include <iostream>
#include <thread>
#include <chrono>


class ShoutGateway : public asgard::com::Gateway{
	using Super = asgard::com::Gateway;
public:
	ShoutGateway(const std::string &name_, std::unique_ptr<asgard::net::Endpoint> endpoint):
		Gateway(name_, std::move(endpoint))
	{
	}

protected:
	void init() override{
		subscribe(input_text);
	}

	void process(std::shared_ptr<const asgard::data::Value> value) override{
		if(auto text = std::dynamic_pointer_cast<const TextLine>(value)){
			process(text);
		}else{
			Super::process(value);
		}
	}

	void process(std::shared_ptr<const TextLine> data){
		shout(data->line);
	}

private:
	void shout(const std::string &message){
		std::string out = message + "\n";
		output_write(out.c_str(), out.length());
	}

	asgard::topic::TopicPtr input_text = "input.terminal";
};



int main(int argc, char **argv){
	if(argc != 2){
		std::cerr << "Usage: " << argv[0] << " endpoint" << std::endl;
		return 1;
	}


	std::unique_ptr<asgard::net::Endpoint> endpoint = asgard::net::Endpoint::from_address(argv[1]);

	asgard::mod::Module::start_module<StdIn>("StdIn");
	asgard::mod::Module::start_module<ShoutGateway>("Gateway", std::move(endpoint));
	//asgard::mod::Module::start_module<asgard::run::Terminal>("Terminal");

	asgard::mod::Module::wait_for_shutdown();

	return 0;
}


