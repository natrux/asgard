#include <ShoutGatewayModule.hxx>


ShoutGatewayModule::ShoutGatewayModule(const std::string &name_):
	Super(name_)
{
}


void ShoutGatewayModule::handle(std::shared_ptr<const asgard::data::Sample> sample){
	auto data = sample->data;
	if(auto d = std::dynamic_pointer_cast<const TextLine>(sample)){
		handle(sample, d);
	}else{
		Super::handle(sample);
	}
}


void ShoutGatewayModule::handle(std::shared_ptr<const asgard::data::Sample> /*sample*/, std::shared_ptr<const TextLine> data){
	handle(data);
}


void ShoutGatewayModule::handle(std::shared_ptr<const TextLine> /*data*/){
	throw std::logic_error("Not implemented");
}

