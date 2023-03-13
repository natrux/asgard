#include <Calculator.h>


Calculator::Calculator(const std::string &name_):
	CalculatorModule(name_)
{
}


void Calculator::main(){
	bind(get_id());
	Super::main();
}


int Calculator::plus_sync(int a, int b) const{
	return a + b;
}


double Calculator::divide_sync(int a, int b) const{
	if(b == 0){
		throw std::logic_error("Division by zero");
	}
	return a / static_cast<double>(b);
}





