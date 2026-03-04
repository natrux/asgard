#pragma once

#include <asgard/data/Message.hxx>
#include <asgard/core/ID.h>

#include <cstddef>
#include <memory>


namespace asgard{
namespace data{


class RPC : public Message{
public:
	size_t message_id;
	core::ID source_address;
	core::ID destination_address;

	void reverse_to(std::shared_ptr<const RPC> other);
};


}
}


