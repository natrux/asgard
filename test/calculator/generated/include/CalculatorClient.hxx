#pragma once

#include <asgard/mod/Client.h>


class CalculatorClient : public asgard::mod::Client{
public:
	CalculatorClient(const asgard::core::ID &destination_);
	CalculatorClient(const std::string &destination_);
	CalculatorClient(asgard::pipe::PipeIn &&destination_);

	int plus(int a, int b);
	void plus_(int a, int b);
	double divide(int a, int b);
	void divide_(int a, int b);
};

