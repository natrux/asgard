#include <asgard/com/NetworkStreamClient.hxx>


namespace asgard{
namespace com{


NetworkStreamClient::NetworkStreamClient(const core::ID &destination_):
	Client(destination_)
{
}


NetworkStreamClient::NetworkStreamClient(const std::string &destination_):
	Client(destination_)
{
}


NetworkStreamClient::NetworkStreamClient(pipe::PipeIn &&destination_):
	Client(std::move(destination_))
{
}


}
}
