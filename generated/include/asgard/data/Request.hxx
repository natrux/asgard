#pragma once

#include <asgard/data/RPC.hxx>
#include <asgard/core/ID.h>

#include <cstddef>


namespace asgard{
namespace data{


class Request : public RPC{
public:
	size_t message_id;
	core::ID source_address;
	core::ID destination_address;
};


}
}


