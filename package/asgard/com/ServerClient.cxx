#include <asgard/com/ServerClient.hxx>


namespace asgard{
namespace com{


ServerClient::ServerClient(const core::ID &destination_):
	Client(destination_)
{
}


ServerClient::ServerClient(const std::string &destination_):
	Client(destination_)
{
}


ServerClient::ServerClient(pipe::PipeIn &&destination_):
	Client(std::move(destination_))
{
}


}
}
