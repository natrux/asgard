#pragma once


#include <asgard/data/Data.hxx>

#include <vector>
#include <cstdint>


namespace asgard{
namespace data{

class DataPacket : public Data{
public:
	std::vector<uint8_t> payload;
};


}
}
