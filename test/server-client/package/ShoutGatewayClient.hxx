#pragma once

#include <asgard/mod/Client.h>


class ShoutGatewayClient : public asgard::mod::Client{
public:
	ShoutGatewayClient(const asgard::core::ID &destination_);
	ShoutGatewayClient(const std::string &destination_);
	ShoutGatewayClient(asgard::pipe::PipeIn &&destination_);
};
