#pragma once

#include <CalculatorModule.hxx>


class Calculator : public CalculatorModule{
public:
	Calculator(const std::string &name_);

protected:
	void main() override;

	int plus_sync(int a, int b) const override;

	double divide_sync(int a, int b) const override;
};

