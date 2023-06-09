#pragma once

#include <Calculator_plus_return.hxx>
#include <Calculator_divide_return.hxx>

#include <asgard/mod/Module.h>
#include <asgard/core/ReturnMe.h>


class CalculatorModule : public asgard::mod::Module{
public:
	using Super = asgard::mod::Module;

	CalculatorModule(const std::string &name_);

protected:
	void process(std::shared_ptr<const asgard::data::Request> request) override;

	virtual void plus_async(int a, int b, asgard::core::ReturnMe<Calculator_plus_return> &&return_me) const;
	virtual int plus_sync(int a, int b) const;

	virtual void divide_async(int a, int b, asgard::core::ReturnMe<Calculator_divide_return> &&return_me) const;
	virtual double divide_sync(int a, int b) const;
};

