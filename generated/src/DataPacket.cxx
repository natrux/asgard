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


void DataPacket::read_from(io::TypeReader &reader, io::typecode_e code){
	if(code == io::typecode_t::TYPE_CLASS){
		const auto size = reader.read_le<uint64_t>();
		for(size_t i=0; i<size; i++){
			std::string name;
			reader.read_type(name);
			if(name == "time"){
				reader.read_type(time);
			}else if(name == "payload"){
				reader.read_type(payload);
			}else{
				reader.skip();
			}
		}
	}else{
		reader.skip(code);
	}
}


void DataPacket::write_to(io::TypeWriter &writer) const{
	const uint64_t size = 2;
	writer.write_le(io::typecode_t::TYPE_CLASS);
	writer.write_le(size);

	writer.write_type("time");
	writer.write_type(time);
	writer.write_type("payload");
	writer.write_type(payload);
}


}
}
