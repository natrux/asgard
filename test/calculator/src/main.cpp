#include <Math.h>
#include <Calculator.h>

#include <asgard/run/Terminal.h>


int main(int /*argc*/, char **/*argv*/){
	asgard::mod::Module::start_module<asgard::run::Terminal>("Terminal");
	asgard::mod::Module::start_module<Calculator>("Calculator");
	asgard::mod::Module::start_module<Math>("Math");


	asgard::mod::Module::wait_for_shutdown();

	return 0;
}




