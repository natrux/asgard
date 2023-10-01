#pragma once

#include <asgard/data/Exception.hxx>
#include <asgard/core/exception.h>

#include <future>


namespace asgard{
namespace core{


template<class Ret>
class ReturnMe{
public:
	ReturnMe(std::shared_ptr<const data::Request> request_) :
		request(request_)
	{
	}

	std::future<std::shared_ptr<const data::Return>> get_future(){
		return promise.get_future();
	}

	template<class ...Args>
	void retrn(Args&&... args){
		auto ret = std::make_shared<Ret>(std::forward<Args>(args)...);
		ret->reverse_to(request);
		promise.set_value(ret);
	}

	void except(const std::exception &err){
		error(err.what());
	}

	void error(const std::string &message){
		auto ex = data::Exception::from_request(request, message);
		except(std::make_exception_ptr(exception(ex)));
	}

private:
	std::shared_ptr<const data::Request> request;
	std::promise<std::shared_ptr<const data::Return>> promise;

	void except(std::exception_ptr error){
		promise.set_exception(error);
	}
};


}
}
