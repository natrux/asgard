#pragma once


#include <asgard/data/Data.hxx>

#include <vector>


namespace asgard{
namespace data{

class DataPacket : public Data{
public:
	std::vector<char> payload;
};


}
}
