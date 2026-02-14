#pragma once

#include <asgard/data/Request.hxx>


class Calculator_plus : public asgard::data::Request{
public:
	Calculator_plus(int a_, int b_);

	int a;
	int b;
};
