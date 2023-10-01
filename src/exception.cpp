#include <asgard/core/exception.h>


namespace asgard{
namespace core{


exception::exception(std::shared_ptr<const data::Exception> error):
	exception_object(error)
{
	if(!exception_object){
		throw std::logic_error("Invalid exception object");
	}
	error_string = exception_object->what();
}


const char *exception::what() const noexcept{
	return error_string.c_str();
}


std::shared_ptr<const data::Exception> exception::get_exception() const{
	return exception_object;
}


}
}

