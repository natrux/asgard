#include <asgard/data/Enum.h>
#include <asgard/io/TypeReader.h>
#include <asgard/io/TypeWriter.h>

#include <stdexcept>


namespace asgard{
namespace data{


code::Typecode Enum::typecode(){
	return code::Typecode(code::Typecode::TYPE_ENUM);
}


}
}
