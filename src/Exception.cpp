#include <asgard/data/Exception.hxx>


namespace asgard{
namespace data{


std::string Exception::what() const{
	return message;
}


}
}
