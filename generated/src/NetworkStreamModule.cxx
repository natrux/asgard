#include <asgard/com/NetworkStreamModule.hxx>


namespace asgard{
namespace com{


NetworkStreamModule::NetworkStreamModule(const std::string &name_):
	Super(name_)
{
}


void NetworkStreamModule::process(std::shared_ptr<const data::Sample> sample){
	auto data = sample->data;
	if(auto d = std::dynamic_pointer_cast<const data::DataPacket>(data)){
		process(sample, d);
	}else{
		Super::process(sample);
	}
}


void NetworkStreamModule::process(std::shared_ptr<const data::Sample> /*sample*/, std::shared_ptr<const data::DataPacket> data){
	process(data);
}


void NetworkStreamModule::process(std::shared_ptr<const data::DataPacket> /*data*/){
	throw std::logic_error("Not implemented");
}


}
}
