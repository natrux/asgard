#include <asgard/com/NetworkStream.h>


namespace asgard{
namespace com{


NetworkStream::NetworkStream(const std::string &name_, std::unique_ptr<net::Endpoint> endpoint):
	Super(name_, std::move(endpoint))
{
}


NetworkStream::NetworkStream(const std::string &name_, const std::string &address):
	Super(name_, address)
{
}


void NetworkStream::init(){
	subscribe(input_data);
}


void NetworkStream::process(std::shared_ptr<const data::Value> value){
	if(auto packet = std::dynamic_pointer_cast<const data::DataPacket>(value)){
		process(packet);
	}else{
		Super::process(value);
	}
}


void NetworkStream::process(std::shared_ptr<const data::DataPacket> data){
	output_write(data->data.data(), data->data.size());
}


void NetworkStream::keep_reading(std::unique_ptr<io::InputSource> input_source){
	while(true){
		auto packet = std::make_shared<data::DataPacket>();
		packet->data.resize(read_buffer_size);
		size_t read = input_source->read(packet->data.data(), read_buffer_size);
		if(read == 0){
			throw std::underflow_error("EOF");
		}
		packet->data.resize(read);
		publish(packet, output_data);
	}
}


void NetworkStream::on_hang_up(){
	// publish empty packet
	auto packet = std::make_shared<data::DataPacket>();
	publish(packet, output_data);
}



}
}
