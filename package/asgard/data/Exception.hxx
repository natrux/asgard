#pragma once


#include <asgard/data/Return.hxx>
#include <asgard/data/Request.hxx>


namespace asgard{
namespace data{


class Exception : public Return{
public:
	static std::shared_ptr<const Exception> from_request(std::shared_ptr<const Request> request, const std::string &message);

	std::string message;

	std::string what() const;
};


}
}
