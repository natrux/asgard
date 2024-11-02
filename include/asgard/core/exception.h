#pragma once

#include <stdexcept>
#include <memory>
#include <asgard/data/Exception.hxx>


namespace asgard{
namespace core{


// Implementation in the header just this once so that we don't need exception.cpp
// which would collide with Exception.cpp on some operating systems.
class exception : public std::exception{
public:
	exception(std::shared_ptr<const data::Exception> error):
		exception_object(error)
	{
		if(!exception_object){
			throw std::logic_error("Invalid exception object");
		}
		error_string = exception_object->what();
	}

	const char *what() const noexcept override{
		return error_string.c_str();
	}

	std::shared_ptr<const data::Exception> get_exception() const{
		return exception_object;
	}

private:
	std::string error_string;
	std::shared_ptr<const data::Exception> exception_object;
};


}
}
