#pragma once


#include <asgard/data/Value.h>

#include <vector>


namespace asgard{
namespace data{

class DataPacket : public Value{
public:
	std::vector<char> data;
};


}
}
