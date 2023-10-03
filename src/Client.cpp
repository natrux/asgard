#include <asgard/mod/Client.h>
#include <asgard/data/Exception.hxx>


namespace asgard{
namespace mod{


Client::Client(const core::ID &destination_):
	Messager(),
	dst_address(destination_)
{
	bind();
}


Client::Client(const std::string &destination_):
	Client(core::ID(destination_))
{
}


Client::Client(pipe::PipeIn &&destination_):
	destination(std::move(destination_))
{
	dst_address.set(0);
	bind();
}


std::future<std::shared_ptr<const data::Return>> Client::call(std::shared_ptr<data::Request> req){
	send_request(req);
	return pending_requests[req->message_id].get_future();
}


void Client::process(std::shared_ptr<const data::Return> ret){
	auto find = pending_requests.find(ret->message_id);
	if(find != pending_requests.end()){
		find->second.set_value(ret);
		pending_requests.erase(find);
		on_return_received(ret);
	}
}


std::shared_ptr<const data::Return> Client::request(std::shared_ptr<data::Request> req){
	auto future = call(req);

	bool has_return = false;
	while(node_should_run() && !has_return){
		process_next();
		if(future.wait_for(std::chrono::seconds::zero()) == std::future_status::ready){
			has_return = true;
		}
	}

	if(has_return){
		std::shared_ptr<const data::Return> result;
		try{
			result = future.get();
		}catch(const std::future_error &err){
			std::string message;
			if(err.code() == std::future_errc::broken_promise){
				message = "Request dropped in client";
			}else{
				message = err.what();
			}
			auto ex = data::Exception::from_request(req, message);
			result = ex;
		}

		return result;
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


size_t Client::last_request_id() const{
	if(num_requests == 0){
		throw std::logic_error("No requests yet");
	}
	return num_requests - 1;
}


}
}



