#pragma once

#include <asgard/net/TcpEndpoint.h>


namespace asgard{
namespace net{


class NamedEndpoint : public Tcp4Endpoint{
public:
	NamedEndpoint(const std::string &name);
};


}
}
