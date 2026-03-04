#pragma once

#include <asgard/com/Gateway.h>
#include <asgard/topic/TopicPtr.h>
#include <asgard/data/Sample.hxx>
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

	using Super::handle;
	void handle(std::shared_ptr<const data::Sample> sample) override;
	virtual void handle(std::shared_ptr<const data::Sample> sample, std::shared_ptr<const data::DataPacket> data);
	virtual void handle(std::shared_ptr<const data::DataPacket> vaue);
};


}
}
