#include <Math.h>
#include <asgard/time/time.h>


Math::Math(const std::string &name_):
	MathModule(name_),
	calc("Calculator"),
	calc_async(std::make_shared<CalculatorClient>("Calculator")),
	generator(device()),
	dist(-5, 5)
{}


void Math::main(){
	set_timer(calculation_interval, std::bind(&Math::calculate_something, this));
	bind_other(calc_async);
	//wait_for_zero = calc_async->wait_for_sum_(0);
	Super::main();
}


void Math::calculate_something(){
	if(!wait_for_zero.valid()){
		wait_for_zero = calc_async->wait_for_sum_(0);
	}
	if(wait_for_zero.wait_for(asgard::time::immediate()) == std::future_status::ready){
		try{
			const auto result = wait_for_zero.get();
			log(INFO) << "Oh look, " << result << " + " << (-result) << " = 0";
		}catch(const std::exception &err){
			log(WARN) << err.what();
		}
		wait_for_zero = calc_async->wait_for_sum_(0);
	}

	const auto a = dist(generator);
	const auto b = dist(generator);

	const auto plus = calc.plus(a, b);
	log(INFO) << a << " + " << b << " = " << plus;

	const auto div = calc.divide(a, b);
	log(INFO) << a << " / " << b << " = " << div;
}



