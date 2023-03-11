#pragma once


#include <asgard/data/Return.h>


namespace asgard{
namespace data{


class Exception : public Return{
public:
	std::string message;

	Exception(const std::string &message_):
		message(message_)
	{}

	std::string what() const{
		return message;
	}
};


}
}
