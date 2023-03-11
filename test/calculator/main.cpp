#include <asgard/mod/Module.h>
#include <asgard/mod/Client.h>
#include <asgard/core/ReturnMe.h>
#include <asgard/run/Terminal.h>

#include <random>


class Calculator_plus : public asgard::data::Request{
public:
	Calculator_plus(int a_, int b_):
		a(a_),
		b(b_)
	{
	}

	int a;
	int b;
};

class Calculator_plus_return : public asgard::data::Return{
public:
	Calculator_plus_return(int result_):
		result(result_)
	{
	}

	int result;
};

class Calculator_divide : public asgard::data::Request{
public:
	Calculator_divide(int a_, int b_):
		a(a_),
		b(b_)
	{
	}

	int a;
	int b;
};

class Calculator_divide_return : public asgard::data::Return{
public:
	Calculator_divide_return(double result_):
		result(result_)
	{
	}

	double result;
};


class Calculator : public asgard::mod::Module{
	using Super = asgard::mod::Module;

public:
	Calculator(const std::string &name_):
		Super(name_)
	{
	}

protected:
	void main() override{
		bind(get_id());
		Super::main();
	}

	void process(std::shared_ptr<const asgard::data::Request> request) override{
		if(auto plus_r = std::dynamic_pointer_cast<const Calculator_plus>(request)){
			plus_async(plus_r->a, plus_r->b, asgard::core::ReturnMe<Calculator_plus_return>(request));
		}else if(auto divide_r = std::dynamic_pointer_cast<const Calculator_divide>(request)){
			divide_async(divide_r->a, divide_r->b, asgard::core::ReturnMe<Calculator_divide_return>(request));
		}else{
			Super::process(request);
		}
	}

	void plus_async(int a, int b, asgard::core::ReturnMe<Calculator_plus_return> &&return_me){
		try{
			const int result = plus_sync(a, b);
			return_me.retrn(result);
		}catch(const std::exception &err){
			return_me.err(err);
		}
	}

	int plus_sync(int a, int b){
		return a + b;
	}

	void divide_async(int a, int b, asgard::core::ReturnMe<Calculator_divide_return> &&return_me){
		try{
			const double result = divide_sync(a, b);
			return_me.retrn(result);
		}catch(const std::exception &err){
			return_me.err(err);
		}
	}

	double divide_sync(int a, int b){
		if(b == 0){
			throw std::logic_error("Division by zero");
		}
		return a / static_cast<double>(b);
	}
};


class CalculatorClient : public asgard::mod::Client{
public:
	CalculatorClient(const asgard::core::ID &destination_):
		Client(destination_)
	{}
	CalculatorClient(const std::string &destination_):
		Client(destination_)
	{}
	CalculatorClient(asgard::pipe::PipeIn &&destination_):
		Client(std::move(destination_))
	{}

	int plus(int a, int b){
		auto ret_ = request_sync<Calculator_plus>(a, b);
		if(auto ret = std::dynamic_pointer_cast<const Calculator_plus_return>(ret_)){
			return ret->result;
		}
		throw std::runtime_error("Return of unexpected type");
	}

	void plus_async(int a, int b){
		request_async<Calculator_plus>(a, b);
	}

	double divide(int a, int b){
		auto ret_ = request_sync<Calculator_divide>(a, b);
		if(auto ret = std::dynamic_pointer_cast<const Calculator_divide_return>(ret_)){
			return ret->result;
		}
		throw std::runtime_error("Return of unexpected type");
	}

	void divide_async(int a, int b){
		request_async<Calculator_divide>(a, b);
	}
};


class Math : public asgard::mod::Module{
	using Super = asgard::mod::Module;

public:
	Math(const std::string &name_):
		Super(name_),
		calc("Calculator"),
		generator(device()),
		dist(-5, 5)
	{}

protected:
	void main() override{
		set_timer(std::chrono::seconds(2), std::bind(&Math::calculate_something, this));
		Super::main();
	}

	void calculate_something(){
		const int a = dist(generator);
		const int b = dist(generator);
		const int plus = calc.plus(a, b);
		log(INFO) << a << " + " << b << " = " << plus;
		const double div = calc.divide(a, b);
		log(INFO) << a << " / " << b << " = " << div;
	}

private:
	CalculatorClient calc;
	std::random_device device;
	std::mt19937 generator;
	std::uniform_int_distribution<> dist;
};






int main(int /*argc*/, char **/*argv*/){
	asgard::mod::Module::start_module<asgard::run::Terminal>("Terminal");
	asgard::mod::Module::start_module<Calculator>("Calculator");
	asgard::mod::Module::start_module<Math>("Math");


	asgard::mod::Module::wait_for_shutdown();

	return 0;
}




