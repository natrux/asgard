#pragma once

#include <asgard/data/Return.hxx>


class Calculator_wait_for_sum_return : public asgard::data::Return{
public:
	Calculator_wait_for_sum_return(const std::pair<int32_t, int32_t> &result_);

	std::pair<int32_t, int32_t> result;
};
