#pragma once

#include <asgard/data/Request.hxx>


class Calculator_divide : public asgard::data::Request{
public:
	Calculator_divide(int a_, int b_);

	int a;
	int b;
};
