#include <asgard/data/Enum.h>
#include <asgard/io/TypeReader.h>
#include <asgard/io/TypeWriter.h>

#include <stdexcept>


namespace asgard{
namespace data{


void Enum::read_from(io::TypeReader &/*reader*/, const io::typecode_t &/*type*/){
	throw std::logic_error("Not implemented");
}


void Enum::write_to(io::TypeWriter &/*writer*/) const{
	throw std::logic_error("Not implemented");
}


}
}
