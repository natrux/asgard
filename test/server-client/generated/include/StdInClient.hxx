#pragma once

#include <asgard/mod/Client.h>


class StdInClient : public asgard::mod::Client{
public:
	StdInClient(const asgard::core::ID &destination_);
	StdInClient(const std::string &destination_);
	StdInClient(asgard::pipe::PipeIn &&destination_);
};
