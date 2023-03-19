#pragma once

#include <asgard/com/Gateway.h>


class EchoGatewayModule : public asgard::com::Gateway{
public:
	using Super = asgard::com::Gateway;

	EchoGatewayModule(const std::string &name_);
};
