#include <ShoutGatewayModule.hxx>
#include <StdIn.h>
#include <asgard/run/Terminal.h>

#include <iostream>


class ShoutGateway : public ShoutGatewayModule{
public:
	ShoutGateway(const std::string &name_, const std::string &address):
		ShoutGatewayModule(name_)
	{
		init_endpoint(address);
	}

	ShoutGateway(const std::string &name_, std::unique_ptr<asgard::net::Endpoint> endpoint):
		ShoutGatewayModule(name_)
	{
		init_endpoint(std::move(endpoint));
	}

protected:
	void init() override{
		subscribe(input_text);
		Super::init();
	}

	void process(std::shared_ptr<const TextLine> data) override{
		shout(data->line);
	}

private:
	void shout(const std::string &message){
		std::string out = message + "\n";
		output_write(out.c_str(), out.length());
	}
};



int main(int argc, char **argv){
	if(argc != 2){
		std::cerr << "Usage: " << argv[0] << " endpoint" << std::endl;
		return 1;
	}
	const std::string address = argv[1];

	asgard::mod::Module::start_module<StdIn>("StdIn");
	//asgard::mod::Module::start_module<asgard::run::Terminal>("Terminal");
	asgard::mod::Module::start_module<ShoutGateway>("Gateway", address);

	asgard::mod::Module::wait_for_shutdown();

	return 0;
}


