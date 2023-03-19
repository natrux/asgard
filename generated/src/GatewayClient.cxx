#include <asgard/com/GatewayClient.hxx>


namespace asgard{
namespace com{


GatewayClient::GatewayClient(const core::ID &destination_):
	Client(destination_)
{
}


GatewayClient::GatewayClient(const std::string &destination_):
	Client(destination_)
{
}


GatewayClient::GatewayClient(pipe::PipeIn &&destination_):
	Client(std::move(destination_))
{
}


}
}
