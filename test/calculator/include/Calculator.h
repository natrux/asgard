#pragma once

#include <CalculatorModule.hxx>


class Calculator : public CalculatorModule{
public:
	Calculator(const std::string &name_);

protected:
	void main() override;

	int plus_sync(int a, int b) const override;

	double divide_sync(int a, int b) const override;

private:
	mutable size_t num_plus = 0;
	mutable size_t num_plus_failed = 0;
	mutable size_t num_div = 0;
	mutable size_t num_div_failed = 0;

	void print_stats() const;
};

