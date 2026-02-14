#include <EchoServerClient.hxx>


EchoServerClient::EchoServerClient(const asgard::core::ID &destination_):
	Client(destination_)
{
}


EchoServerClient::EchoServerClient(const std::string &destination_):
	Client(destination_)
{
}


EchoServerClient::EchoServerClient(asgard::pipe::PipeIn &&destination_):
	Client(std::move(destination_))
{
}
