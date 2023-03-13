#pragma once

#include <MathModule.hxx>
#include <CalculatorClient.hxx>

#include <random>


class Math : public MathModule{
public:
	Math(const std::string &name_);

protected:
	void main() override;

private:
	CalculatorClient calc;
	std::random_device device;
	std::mt19937 generator;
	std::uniform_int_distribution<> dist;

	void calculate_something();
};
