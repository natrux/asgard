#include <asgard/data/DataPacket.hxx>
#include <asgard/io/TypeReader.h>
#include <asgard/io/TypeWriter.h>


namespace asgard{
namespace data{


bool DataPacket::operator==(const DataPacket &other) const{
	return
		time == other.time &&
		payload == other.payload &&
	true;
}


}
}
