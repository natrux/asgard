#include <asgard/com/NetworkStream.h>


namespace asgard{
namespace com{


NetworkStream::NetworkStream(const std::string &name_):
	NetworkStreamModule(name_)
{
}


NetworkStream::NetworkStream(const std::string &name_, std::unique_ptr<net::Endpoint> endpoint):
	NetworkStreamModule(name_)
{
	init_endpoint(std::move(endpoint));
}


NetworkStream::NetworkStream(const std::string &name_, const std::string &address):
	NetworkStream(name_, net::Endpoint::from_address(address))
{
}


void NetworkStream::init(){
	subscribe(input_data);
	Super::init();
}


void NetworkStream::process(std::shared_ptr<const data::DataPacket> data){
	output_write(data->payload.data(), data->payload.size());
}


void NetworkStream::keep_reading(std::unique_ptr<io::InputSource> input_source){
	while(true){
		auto packet = std::make_shared<data::DataPacket>();
		packet->payload.resize(read_buffer_size);
		const size_t read = input_source->read(packet->payload.data(), read_buffer_size);
		if(read == 0){
			throw std::underflow_error("EOF");
		}
		packet->payload.resize(read);
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
