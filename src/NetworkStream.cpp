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
