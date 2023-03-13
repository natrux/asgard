#include <MathClient.hxx>


MathClient::MathClient(const asgard::core::ID &destination_):
	Client(destination_)
{
}


MathClient::MathClient(const std::string &destination_):
	Client(destination_)
{
}


MathClient::MathClient(asgard::pipe::PipeIn &&destination_):
	Client(std::move(destination_))
{
}
