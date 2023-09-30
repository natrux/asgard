#pragma once

#include <CalculatorModule.hxx>


class Calculator : public CalculatorModule{
public:
	Calculator(const std::string &name_);

protected:
	void main() override;

	int plus_sync(int a, int b) const override;

	double divide_sync(int a, int b) const override;

	void wait_for_sum_async(int sum, asgard::core::ReturnMe<Calculator_wait_for_sum_return> &&return_me) const override;

private:
	mutable size_t num_plus = 0;
	mutable size_t num_plus_failed = 0;
	mutable size_t num_div = 0;
	mutable size_t num_div_failed = 0;

	mutable std::map<int, std::vector<asgard::core::ReturnMe<Calculator_wait_for_sum_return>>> waiting_for_sum;

	void print_stats() const;
};

