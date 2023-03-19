#include <ShoutGatewayClient.hxx>


ShoutGatewayClient::ShoutGatewayClient(const asgard::core::ID &destination_):
	Client(destination_)
{
}


ShoutGatewayClient::ShoutGatewayClient(const std::string &destination_):
	Client(destination_)
{
}


ShoutGatewayClient::ShoutGatewayClient(asgard::pipe::PipeIn &&destination_):
	Client(std::move(destination_))
{
}
