#include <CalculatorModule.hxx>
#include <Calculator_plus.hxx>
#include <Calculator_divide.hxx>


CalculatorModule::CalculatorModule(const std::string &name_):
	Super(name_)
{
}


void CalculatorModule::process(std::shared_ptr<const asgard::data::Request> request){
	if(auto plus_r = std::dynamic_pointer_cast<const Calculator_plus>(request)){
		asgard::core::ReturnMe<Calculator_plus_return> return_me;
		add_pending_request(request, return_me.get_future());
		plus_async(plus_r->a, plus_r->b, std::move(return_me));
	}else if(auto divide_r = std::dynamic_pointer_cast<const Calculator_divide>(request)){
		asgard::core::ReturnMe<Calculator_divide_return> return_me;
		add_pending_request(request, return_me.get_future());
		divide_async(divide_r->a, divide_r->b, std::move(return_me));
	}else{
		Super::process(request);
	}
}


void CalculatorModule::plus_async(int a, int b, asgard::core::ReturnMe<Calculator_plus_return> &&return_me) const{
	try{
		const int result = plus_sync(a, b);
		return_me.retrn(result);
	}catch(...){
		return_me.except(std::current_exception());
	}
}


int CalculatorModule::plus_sync(int /*a*/, int /*b*/) const{
	throw std::logic_error("Not implemented");
}


void CalculatorModule::divide_async(int a, int b, asgard::core::ReturnMe<Calculator_divide_return> &&return_me) const{
	try{
		const double result = divide_sync(a, b);
		return_me.retrn(result);
	}catch(...){
		return_me.except(std::current_exception());
	}
}


double CalculatorModule::divide_sync(int /*a*/, int /*b*/) const{
	throw std::logic_error("Not implemented");
}





