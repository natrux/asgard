#include <EchoGatewayClient.hxx>


EchoGatewayClient::EchoGatewayClient(const asgard::core::ID &destination_):
	Client(destination_)
{
}


EchoGatewayClient::EchoGatewayClient(const std::string &destination_):
	Client(destination_)
{
}


EchoGatewayClient::EchoGatewayClient(asgard::pipe::PipeIn &&destination_):
	Client(std::move(destination_))
{
}
