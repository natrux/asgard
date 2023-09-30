#include <Math.h>


Math::Math(const std::string &name_):
	MathModule(name_),
	calc("Calculator"),
	calc_async(std::make_shared<CalculatorClient>("Calculator")),
	generator(device()),
	dist(-5, 5)
{}


void Math::main(){
	set_timer(std::chrono::seconds(2), std::bind(&Math::calculate_something, this));
	bind_other(calc_async);
	//wait_for_zero = calc_async->wait_for_sum_(0);
	Super::main();
}


void Math::calculate_something(){
	if(!wait_for_zero.valid()){
		wait_for_zero = calc_async->wait_for_sum_(0);
	}
	if(wait_for_zero.wait_for(std::chrono::seconds::zero()) == std::future_status::ready){
		try{
			const auto result = wait_for_zero.get();
			log(INFO) << "Oh look, " << result << " + " << (-result) << " = 0";
		}catch(const std::exception &err){
			log(WARN) << err.what();
		}
		wait_for_zero = calc_async->wait_for_sum_(0);
	}

	const int a = dist(generator);
	const int b = dist(generator);

	const int plus = calc.plus(a, b);
	log(INFO) << a << " + " << b << " = " << plus;

	const double div = calc.divide(a, b);
	log(INFO) << a << " / " << b << " = " << div;
}



