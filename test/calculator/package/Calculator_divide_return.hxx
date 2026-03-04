#pragma once

#include <asgard/data/Return.hxx>


class Calculator_divide_return : public asgard::data::Return{
public:
	Calculator_divide_return(double result_);

	double result;
};

