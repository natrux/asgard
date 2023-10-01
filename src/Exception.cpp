#include <asgard/data/Exception.hxx>


namespace asgard{
namespace data{


std::shared_ptr<const Exception> Exception::from_request(std::shared_ptr<const Request> request, const std::string &message){
	auto ex = std::make_shared<Exception>();
	ex->reverse_to(request);
	ex->message = message;
	return ex;
}


std::string Exception::what() const{
	return message;
}


}
}
