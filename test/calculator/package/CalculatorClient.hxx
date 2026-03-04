#pragma once

#include <asgard/mod/Client.h>


class CalculatorClient : public asgard::mod::Client{
	using Super = asgard::mod::Client;
public:
	CalculatorClient(const asgard::core::ID &destination_);
	CalculatorClient(const std::string &destination_);
	CalculatorClient(asgard::pipe::PipeIn &&destination_);

	int32_t plus(const int32_t &a, const int32_t &b);
	std::future<int32_t> plus_(const int32_t &a, const int32_t &b);
	double divide(const int32_t &a, const int32_t &b);
	std::future<double> divide_(const int32_t &a, const int32_t &b);
	int32_t wait_for_sum(const int32_t &sum);
	std::future<int32_t> wait_for_sum_(const int32_t &sum);

private:
	std::map<size_t, std::promise<int>> pending_requests_plus;
	std::map<size_t, std::promise<double>> pending_requests_divide;
	std::map<size_t, std::promise<int>> pending_requests_wait_for_sum;

	void on_return_received(std::shared_ptr<const asgard::data::Return> ret) override;
};
