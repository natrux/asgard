#include <asgard/data/RPC.hxx>


namespace asgard{
namespace data{


void RPC::reverse_to(std::shared_ptr<const RPC> other){
	message_id = other->message_id;
	source_address = other->destination_address;
	destination_address = other->source_address;
}


}
}
