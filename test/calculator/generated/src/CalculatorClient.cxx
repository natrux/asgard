#include <CalculatorClient.hxx>
#include <Calculator_plus.hxx>
#include <Calculator_plus_return.hxx>
#include <Calculator_divide.hxx>
#include <Calculator_divide_return.hxx>


CalculatorClient::CalculatorClient(const asgard::core::ID &destination_):
	Client(destination_)
{
}


CalculatorClient::CalculatorClient(const std::string &destination_):
	Client(destination_)
{
}


CalculatorClient::CalculatorClient(asgard::pipe::PipeIn &&destination_):
	Client(std::move(destination_))
{
}


int CalculatorClient::plus(int a, int b){
	auto ret_ = request<Calculator_plus>(a, b);
	if(auto ret = std::dynamic_pointer_cast<const Calculator_plus_return>(ret_)){
		return ret->result;
	}
	throw std::runtime_error("Return of unexpected type");
}


void CalculatorClient::plus_(int a, int b){
	call<Calculator_plus>(a, b);
}


double CalculatorClient::divide(int a, int b){
	auto ret_ = request<Calculator_divide>(a, b);
	if(auto ret = std::dynamic_pointer_cast<const Calculator_divide_return>(ret_)){
		return ret->result;
	}
	throw std::runtime_error("Return of unexpected type");
}


void CalculatorClient::divide_(int a, int b){
	call<Calculator_divide>(a, b);
}
