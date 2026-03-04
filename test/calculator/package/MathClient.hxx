#pragma once

#include <asgard/mod/Client.h>


class MathClient : public asgard::mod::Client{
public:
	MathClient(const asgard::core::ID &destination_);
	MathClient(const std::string &destination_);
	MathClient(asgard::pipe::PipeIn &&destination_);
};
