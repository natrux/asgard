#pragma once

#include <asgard/mod/Client.h>


namespace asgard{
namespace com{


class GatewayClient : public mod::Client{
public:
	GatewayClient(const core::ID &destination);
	GatewayClient(const std::string &destination);
	GatewayClient(pipe::PipeIn &&destination);
};


}
}
