#include <Math.h>


Math::Math(const std::string &name_):
	MathModule(name_),
	calc("Calculator"),
	generator(device()),
	dist(-5, 5)
{}


void Math::main(){
	set_timer(std::chrono::seconds(2), std::bind(&Math::calculate_something, this));
	Super::main();
}


void Math::calculate_something(){
	const int a = dist(generator);
	const int b = dist(generator);
	const int plus = calc.plus(a, b);
	log(INFO) << a << " + " << b << " = " << plus;
	const double div = calc.divide(a, b);
	log(INFO) << a << " / " << b << " = " << div;
}



