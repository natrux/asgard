#pragma once


#include <asgard/data/Return.hxx>


namespace asgard{
namespace data{


class Exception : public Return{
public:
	std::string message;

	std::string what() const;
};


}
}
