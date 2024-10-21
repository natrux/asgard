#pragma once

#include <asgard/data/Exception.hxx>
#include <asgard/data/Interrupt.hxx>
#include <asgard/core/exception.h>

#include <future>


namespace asgard{
namespace core{


template<class Ret>
class ReturnMe{
public:
	ReturnMe(std::shared_ptr<const data::Request> request_, pipe::PipeIn &&destination_) :
		request(request_),
		destination(std::move(destination_))
	{
	}

	ReturnMe(ReturnMe &&other) = default;

	~ReturnMe(){
		// notify the owner
		// explicitly break the promise to avoid race condition
		promise = {};
		interrupt();
	}

	ReturnMe &operator=(ReturnMe &&other) = default;

	std::future<std::shared_ptr<const data::Return>> get_future(){
		return promise.get_future();
	}

	template<class ...Args>
	void retrn(Args&&... args){
		auto ret = std::make_shared<Ret>(std::forward<Args>(args)...);
		ret->reverse_to(request);
		promise.set_value(ret);
		interrupt();
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
	pipe::PipeIn destination;
	std::promise<std::shared_ptr<const data::Return>> promise;

	void except(std::exception_ptr error){
		promise.set_exception(error);
		interrupt();
	}

	void interrupt(){
		try{
			destination.push(std::make_shared<data::Interrupt>());
		}catch(const std::exception &){
		}
	}
};


}
}
