#pragma once

#include <asgard/com/Gateway.h>
#include <asgard/topic/TopicPtr.h>
#include <asgard/data/Value.h>
#include <asgard/data/DataPacket.hxx>


namespace asgard{
namespace com{


class NetworkStreamModule : public Gateway{
public:
	using Super = Gateway;

	NetworkStreamModule(const std::string &name_);

protected:
	topic::TopicPtr input_data;
	topic::TopicPtr output_data;
	size_t read_buffer_size = 1024;

	using Super::process;
	void process(std::shared_ptr<const data::Value> value) override;
	virtual void process(std::shared_ptr<const data::DataPacket> vaue) = 0;
};


}
}
