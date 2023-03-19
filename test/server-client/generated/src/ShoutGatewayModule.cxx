#include <ShoutGatewayModule.hxx>


ShoutGatewayModule::ShoutGatewayModule(const std::string &name_):
	Super(name_)
{
}


void ShoutGatewayModule::process(std::shared_ptr<const asgard::data::Value> value){
	if(auto value_ = std::dynamic_pointer_cast<const TextLine>(value)){
		process(value_);
	}else{
		Super::process(value);
	}
}

