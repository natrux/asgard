#pragma once

#include <asgard/com/NetworkStreamModule.hxx>


namespace asgard{
namespace com{


class NetworkStream : public NetworkStreamModule{
public:
	NetworkStream(const std::string &name_);
	NetworkStream(const std::string &name_, std::unique_ptr<net::Endpoint> endpoint);
	NetworkStream(const std::string &name_, const std::string &address);

protected:
	void main() override;

	void process(std::shared_ptr<const data::DataPacket> data) override;

private:
	void keep_reading(std::unique_ptr<io::InputSource> input_source) override;
	void on_hang_up() override;
};



}
}
