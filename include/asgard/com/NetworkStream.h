#pragma once

#include <asgard/com/Gateway.h>
#include <asgard/topic/TopicPtr.h>
#include <asgard/data/DataPacket.h>


namespace asgard{
namespace com{


class NetworkStream : public Gateway{
	using Super = Gateway;
public:
	NetworkStream(const std::string &name_, std::unique_ptr<net::Endpoint> endpoint);
	NetworkStream(const std::string &name_, const std::string &address);

protected:
	void init() override;
	void process(std::shared_ptr<const data::Value> value) override;
	void process(std::shared_ptr<const data::DataPacket> data);

private:
	topic::TopicPtr input_data;
	topic::TopicPtr output_data;
	size_t read_buffer_size = 4096;

	void keep_reading(std::unique_ptr<io::InputSource> input_source) override;
	void on_hang_up() override;
};



}
}
