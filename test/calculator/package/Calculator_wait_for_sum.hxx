#pragma once

#include <asgard/data/Request.hxx>


class Calculator_wait_for_sum : public asgard::data::Request{
public:
	Calculator_wait_for_sum(int sum_);

	int sum;
};
