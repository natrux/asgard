#include <asgard/com/NetworkStreamModule.hxx>


namespace asgard{
namespace com{


NetworkStreamModule::NetworkStreamModule(const std::string &name_):
	Super(name_)
{
}


void NetworkStreamModule::process(std::shared_ptr<const data::Value> value){
	if(auto data_packet = std::dynamic_pointer_cast<const data::DataPacket>(value)){
		process(data_packet);
	}else{
		Super::process(value);
	}
}


}
}
