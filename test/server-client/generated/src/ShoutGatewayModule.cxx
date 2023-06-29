#include <ShoutGatewayModule.hxx>


ShoutGatewayModule::ShoutGatewayModule(const std::string &name_):
	Super(name_)
{
}


void ShoutGatewayModule::process(std::shared_ptr<const asgard::data::Sample> sample){
	auto data = sample->data;
	if(auto d = std::dynamic_pointer_cast<const TextLine>(sample)){
		process(sample, d);
	}else{
		Super::process(sample);
	}
}


void ShoutGatewayModule::process(std::shared_ptr<const asgard::data::Sample> /*sample*/, std::shared_ptr<const TextLine> data){
	process(data);
}


void ShoutGatewayModule::process(std::shared_ptr<const TextLine> /*data*/){
	throw std::logic_error("Not implemented");
}

