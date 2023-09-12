#include <Calculator.h>


Calculator::Calculator(const std::string &name_):
	CalculatorModule(name_)
{
}


void Calculator::main(){
	bind(get_id());
	set_timer(std::chrono::milliseconds(stats_interval_ms), std::bind(&Calculator::print_stats, this));
	Super::main();
}


int Calculator::plus_sync(int a, int b) const{
	num_plus++;
	return a + b;
}


double Calculator::divide_sync(int a, int b) const{
	num_div++;
	if(b == 0){
		num_div_failed++;
		throw std::logic_error("Division by zero");
	}
	return a / static_cast<double>(b);
}


void Calculator::print_stats() const{
	log(INFO) << (num_plus - num_plus_failed) << "/" << num_plus << " additions, " << (num_div - num_div_failed) << "/" << num_div << " divisions successful";
}





