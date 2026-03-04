#pragma once

#include <Calculator_plus_return.hxx>
#include <Calculator_divide_return.hxx>
#include <Calculator_wait_for_sum_return.hxx>

#include <asgard/mod/Module.h>
#include <asgard/time/time.h>
#include <asgard/core/ReturnMe.h>


class CalculatorModule : public asgard::mod::Module{
public:
	using Super = asgard::mod::Module;

	CalculatorModule(const std::string &name_);

protected:
	asgard::time::duration stats_interval = 5s;

	void handle(std::shared_ptr<const asgard::data::Request> request) override;

	virtual void plus_async(const int32_t &a, const int32_t &b, asgard::core::ReturnMe<Calculator_plus_return> &&return_me) const;
	virtual int32_t plus_sync(const int32_t &a, const int32_t &b) const;

	virtual void divide_async(const int32_t &a, const int &b, asgard::core::ReturnMe<Calculator_divide_return> &&return_me) const;
	virtual double divide_sync(const int32_t &a, const int32_t &b) const;

	virtual void wait_for_sum_async(const int32_t &sum, asgard::core::ReturnMe<Calculator_wait_for_sum_return> &&return_me) const;
	virtual int32_t wait_for_sum_sync(const int32_t &sum) const;
};

