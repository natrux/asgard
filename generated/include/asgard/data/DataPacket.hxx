#pragma once


#include <asgard/data/Data.hxx>
#include <asgard/time/time.h>

#include <vector>
#include <cstdint>


namespace asgard{
namespace data{

class DataPacket : public Data{
public:
	time::time time;
	std::vector<uint8_t> payload;

	bool operator==(const DataPacket &other) const;

	void read_from(io::TypeReader &reader, io::typecode_e code);
	void write_to(io::TypeWriter &writer) const;
};


}
}
