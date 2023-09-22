#pragma once

#include <asgard/mod/Client.h>


class CalculatorClient : public asgard::mod::Client{
	using Super = asgard::mod::Client;
public:
	CalculatorClient(const asgard::core::ID &destination_);
	CalculatorClient(const std::string &destination_);
	CalculatorClient(asgard::pipe::PipeIn &&destination_);

	int plus(int a, int b);
	std::future<int> plus_(int a, int b);
	double divide(int a, int b);
	std::future<double> divide_(int a, int b);

private:
	std::map<size_t, std::promise<int>> pending_requests_plus;
	std::map<size_t, std::promise<double>> pending_requests_divide;

	void on_return_received(std::shared_ptr<const asgard::data::Return> ret) override;
};
