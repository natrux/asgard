#include <CalculatorModule.hxx>
#include <Calculator_plus.hxx>
#include <Calculator_divide.hxx>


CalculatorModule::CalculatorModule(const std::string &name_):
	Super(name_)
{
}


void CalculatorModule::process(std::shared_ptr<const asgard::data::Request> request){
	if(auto plus_r = std::dynamic_pointer_cast<const Calculator_plus>(request)){
		plus_async(plus_r->a, plus_r->b, asgard::core::ReturnMe<Calculator_plus_return>(request));
	}else if(auto divide_r = std::dynamic_pointer_cast<const Calculator_divide>(request)){
		divide_async(divide_r->a, divide_r->b, asgard::core::ReturnMe<Calculator_divide_return>(request));
	}else{
		Super::process(request);
	}
}


void CalculatorModule::plus_async(int a, int b, asgard::core::ReturnMe<Calculator_plus_return> &&return_me) const{
	try{
		const int result = plus_sync(a, b);
		return_me.retrn(result);
	}catch(const std::exception &err){
		return_me.err(err);
	}
}


int CalculatorModule::plus_sync(int /*a*/, int /*b*/) const{
	throw std::logic_error("Not implemented");
}


void CalculatorModule::divide_async(int a, int b, asgard::core::ReturnMe<Calculator_divide_return> &&return_me) const{
	try{
		const double result = divide_sync(a, b);
		return_me.retrn(result);
	}catch(const std::exception &err){
		return_me.err(err);
	}
}


double CalculatorModule::divide_sync(int /*a*/, int /*b*/) const{
	throw std::logic_error("Not implemented");
}





