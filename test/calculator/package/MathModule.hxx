#pragma once

#include <asgard/mod/Module.h>


class MathModule : public asgard::mod::Module{
public:
	using Super = asgard::mod::Module;

	MathModule(const std::string &name_);

protected:
	asgard::time::duration calculation_interval = 2s;
};

