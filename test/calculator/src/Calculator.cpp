#include <Calculator.h>


Calculator::Calculator(const std::string &name_):
	CalculatorModule(name_)
{
}


void Calculator::main(){
	bind();
	set_timer(stats_interval, std::bind(&Calculator::print_stats, this));
	Super::main();
}


int32_t Calculator::plus_sync(const int32_t &a, const int32_t &b) const{
	num_plus++;
	const auto sum = a + b;

	{
		const auto find = waiting_for_sum.find(sum);
		if(find != waiting_for_sum.end()){
			for(auto &r : find->second){
				r.retrn(a);
			}
			waiting_for_sum.erase(find);
		}
	}

	return sum;
}


double Calculator::divide_sync(const int32_t &a, const int32_t &b) const{
	num_div++;
	if(b == 0){
		num_div_failed++;
		throw std::logic_error("Division by zero");
	}
	return a / static_cast<double>(b);
}


void Calculator::wait_for_sum_async(const int32_t &sum, asgard::core::ReturnMe<Calculator_wait_for_sum_return> &&return_me) const{
	waiting_for_sum[sum].push_back(std::move(return_me));
}


void Calculator::print_stats() const{
	log(INFO) << (num_plus - num_plus_failed) << "/" << num_plus << " additions, " << (num_div - num_div_failed) << "/" << num_div << " divisions successful";
}





