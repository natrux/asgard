#include <CalculatorModule.hxx>
#include <Calculator_plus.hxx>
#include <Calculator_divide.hxx>
#include <Calculator_wait_for_sum.hxx>


CalculatorModule::CalculatorModule(const std::string &name_):
	Super(name_)
{
}


void CalculatorModule::process(std::shared_ptr<const asgard::data::Request> request){
	if(auto plus_r = std::dynamic_pointer_cast<const Calculator_plus>(request)){
		asgard::core::ReturnMe<Calculator_plus_return> return_me(request, make_pipe_in());
		add_pending_request(request, return_me.get_future());
		plus_async(plus_r->a, plus_r->b, std::move(return_me));
	}else if(auto divide_r = std::dynamic_pointer_cast<const Calculator_divide>(request)){
		asgard::core::ReturnMe<Calculator_divide_return> return_me(request, make_pipe_in());
		add_pending_request(request, return_me.get_future());
		divide_async(divide_r->a, divide_r->b, std::move(return_me));
	}else if(auto wait_for_sum_r = std::dynamic_pointer_cast<const Calculator_wait_for_sum>(request)){
		asgard::core::ReturnMe<Calculator_wait_for_sum_return> return_me(request, make_pipe_in());
		add_pending_request(request, return_me.get_future());
		wait_for_sum_async(wait_for_sum_r->sum, std::move(return_me));
	}else{
		Super::process(request);
	}
}


void CalculatorModule::plus_async(const int32_t &a, const int32_t &b, asgard::core::ReturnMe<Calculator_plus_return> &&return_me) const{
	try{
		const int32_t result = plus_sync(a, b);
		return_me.retrn(result);
	}catch(const std::exception &err){
		return_me.except(err);
	}
}


int32_t CalculatorModule::plus_sync(const int32_t &/*a*/, const int32_t &/*b*/) const{
	throw std::logic_error("Not implemented");
}


void CalculatorModule::divide_async(const int32_t &a, const int32_t &b, asgard::core::ReturnMe<Calculator_divide_return> &&return_me) const{
	try{
		const double result = divide_sync(a, b);
		return_me.retrn(result);
	}catch(const std::exception &err){
		return_me.except(err);
	}
}


double CalculatorModule::divide_sync(const int32_t &/*a*/, const int32_t &/*b*/) const{
	throw std::logic_error("Not implemented");
}


void CalculatorModule::wait_for_sum_async(const int32_t &sum, asgard::core::ReturnMe<Calculator_wait_for_sum_return> &&return_me) const{
	try{
		const int32_t result = wait_for_sum_sync(sum);
		return_me.retrn(result);
	}catch(const std::exception &err){
		return_me.except(err);
	}
}


int32_t CalculatorModule::wait_for_sum_sync(const int32_t &/*sum*/) const{
	throw std::logic_error("Not implemented");
}





