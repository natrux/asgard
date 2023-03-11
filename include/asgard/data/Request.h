#pragma once


#include <asgard/data/Data.h>


namespace asgard{
namespace data{


class Request : public Data{
public:
	size_t message_id;
	core::ID source_address;
	core::ID destination_address;
	bool needs_return = true;
};


}
}


