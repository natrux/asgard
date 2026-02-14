#include <StdInClient.hxx>


StdInClient::StdInClient(const asgard::core::ID &destination_):
	Client(destination_)
{
}


StdInClient::StdInClient(const std::string &destination_):
	Client(destination_)
{
}


StdInClient::StdInClient(asgard::pipe::PipeIn &&destination_):
	Client(std::move(destination_))
{
}
