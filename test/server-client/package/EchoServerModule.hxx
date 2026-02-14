#pragma once

#include <asgard/com/Server.h>


class EchoServerModule : public asgard::com::Server{
public:
	using Super = asgard::com::Server;

	EchoServerModule(const std::string &name_);
};

