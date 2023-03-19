#pragma once

#include <asgard/mod/Client.h>


class EchoServerClient : public asgard::mod::Client{
public:
	EchoServerClient(const asgard::core::ID &destination_);
	EchoServerClient(const std::string &destination_);
	EchoServerClient(asgard::pipe::PipeIn &&destination_);
};
