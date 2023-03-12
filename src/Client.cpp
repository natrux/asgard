#include <asgard/mod/Client.h>
#include <asgard/data/Exception.h>


namespace asgard{
namespace mod{


Client::Client(const core::ID &destination_):
	Messager(),
	dst_address(destination_)
{
	bind(get_id());
}


Client::Client(const std::string &destination_):
	Client(core::ID(destination_))
{
}


Client::Client(pipe::PipeIn &&destination_):
	destination(std::move(destination_))
{
	dst_address.set(0);
	bind(get_id());
}


void Client::call(std::shared_ptr<data::Request> req){
	req->needs_return = false;
	send_request(req);
}


void Client::process(std::shared_ptr<const data::Return> ret){
	if(ret->message_id == current_request->message_id){
		current_return = ret;
	}
}


std::shared_ptr<const data::Return> Client::request(std::shared_ptr<data::Request> req){
	current_request = req;
	current_return = nullptr;
	send_request(req);

	while(node_should_run() && !current_return){
		process_next();
	}
	if(current_return){
		if(auto ex = std::dynamic_pointer_cast<const data::Exception>(current_return)){
			throw std::runtime_error(ex->what());
		}
		return current_return;
	}
	return nullptr;
}


void Client::send_request(std::shared_ptr<data::Request> req){
	req->message_id = num_requests++;
	req->source_address = get_id();
	req->destination_address = dst_address;
	if(!destination.is_connected()){
		destination = pipe::Pipe::get(dst_address);
	}
	destination.push(req);
}


}
}



