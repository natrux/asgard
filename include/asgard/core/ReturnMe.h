#pragma once

#include <asgard/data/Exception.hxx>

#include <future>


namespace asgard{
namespace core{


template<class Ret>
class ReturnMe{
public:
	std::future<std::shared_ptr<data::Return>> get_future(){
		return promise.get_future();
	}

	template<class ...Args>
	void retrn(Args&&... args){
		auto ret = std::make_shared<Ret>(std::forward<Args>(args)...);
		promise.set_value(ret);
	}

	template<class E>
	void except(const E &err){
		except(std::make_exception_ptr(err));
	}

	void except(const std::exception &err){
		// Special treatment for std::exception because the message gets lost due to slicing
		error(err.what());
	}

	void except(std::exception_ptr error){
		promise.set_exception(error);
	}

	void error(const std::string &message){
		except(std::make_exception_ptr(std::runtime_error(message)));
	}

	void error(const char *message){
		error(std::string(message));
	}

private:
	std::promise<std::shared_ptr<data::Return>> promise;
};


}
}
