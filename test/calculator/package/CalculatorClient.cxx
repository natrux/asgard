#include <CalculatorClient.hxx>
#include <Calculator_plus.hxx>
#include <Calculator_plus_return.hxx>
#include <Calculator_divide.hxx>
#include <Calculator_divide_return.hxx>
#include <Calculator_wait_for_sum.hxx>
#include <Calculator_wait_for_sum_return.hxx>
#include <asgard/data/Exception.hxx>
#include <asgard/core/exception.h>


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


int32_t CalculatorClient::plus(const int32_t &a, const int32_t &b){
	auto ret_ = request<Calculator_plus>(a, b);
	if(auto ret = std::dynamic_pointer_cast<const Calculator_plus_return>(ret_)){
		return ret->result;
	}else if(auto ex = std::dynamic_pointer_cast<const asgard::data::Exception>(ret_)){
		throw asgard::core::exception(ex);
	}
	throw std::runtime_error("Return of unexpected type");
}


std::future<int32_t> CalculatorClient::plus_(const int32_t &a, const int32_t &b){
	call<Calculator_plus>(a, b);
	return pending_requests_plus[last_request_id()].get_future();
}


double CalculatorClient::divide(const int32_t &a, const int32_t &b){
	auto ret_ = request<Calculator_divide>(a, b);
	if(auto ret = std::dynamic_pointer_cast<const Calculator_divide_return>(ret_)){
		return ret->result;
	}else if(auto ex = std::dynamic_pointer_cast<const asgard::data::Exception>(ret_)){
		throw asgard::core::exception(ex);
	}
	throw std::runtime_error("Return of unexpected type");
}


std::future<double> CalculatorClient::divide_(const int32_t &a, const int32_t &b){
	call<Calculator_divide>(a, b);
	return pending_requests_divide[last_request_id()].get_future();
}


int32_t CalculatorClient::wait_for_sum(const int32_t &sum){
	auto ret_ = request<Calculator_wait_for_sum>(sum);
	if(auto ret = std::dynamic_pointer_cast<const Calculator_wait_for_sum_return>(ret_)){
		return ret->result;
	}else if(auto ex = std::dynamic_pointer_cast<const asgard::data::Exception>(ret_)){
		throw asgard::core::exception(ex);
	}
	throw std::runtime_error("Return of unexpected type");
}


std::future<int32_t> CalculatorClient::wait_for_sum_(const int32_t &sum){
	call<Calculator_wait_for_sum>(sum);
	return pending_requests_wait_for_sum[last_request_id()].get_future();
}


void CalculatorClient::on_return_received(std::shared_ptr<const asgard::data::Return> ret){
	Super::on_return_received(ret);
	return_dispatch_helper<Calculator_plus_return>(ret, pending_requests_plus);
	return_dispatch_helper<Calculator_divide_return>(ret, pending_requests_divide);
	return_dispatch_helper<Calculator_wait_for_sum_return>(ret, pending_requests_wait_for_sum);
}
