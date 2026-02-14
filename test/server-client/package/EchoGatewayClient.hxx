#pragma once

#include <asgard/mod/Client.h>


class EchoGatewayClient : public asgard::mod::Client{
public:
	EchoGatewayClient(const asgard::core::ID &destination_);
	EchoGatewayClient(const std::string &destination_);
	EchoGatewayClient(asgard::pipe::PipeIn &&destination_);
};
