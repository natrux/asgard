#pragma once

#include <asgard/data/Return.h>


class Calculator_plus_return : public asgard::data::Return{
public:
	Calculator_plus_return(int result_);

	int result;
};
