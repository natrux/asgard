#include <asgard/net/NamedEndpoint.h>


namespace asgard{
namespace net{


NamedEndpoint::NamedEndpoint(const std::string &name):
	Tcp4Endpoint("localhost", (std::hash<std::string>{}(name) % 10000) + 30000)
{
}



}
}

