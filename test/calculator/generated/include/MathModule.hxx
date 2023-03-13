#pragma once

#include <asgard/mod/Module.h>


class MathModule : public asgard::mod::Module{
public:
	using Super = asgard::mod::Module;

	MathModule(const std::string &name_);
};

