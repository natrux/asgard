#pragma once

#include <asgard/data/Return.hxx>


class Calculator_wait_for_sum_return : public asgard::data::Return{
public:
	Calculator_wait_for_sum_return(int result_);

	int result;
};
