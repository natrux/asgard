#pragma once

#include <asgard/data/Exception.h>


namespace asgard{
namespace core{


template<class Ret>
class ReturnMe{
public:
	ReturnMe(std::shared_ptr<const data::Request> to_request):
		request(to_request)
	{
	}

	ReturnMe(const ReturnMe &other) = delete;
	ReturnMe(ReturnMe &&other){
		*this = std::move(other);
	}

	ReturnMe &operator=(const ReturnMe &other) = delete;
	ReturnMe &operator=(ReturnMe &&other){
		request = std::move(other.request);
		done = other.done;
		other.done = true;
	}

	template<class ...Args>
	void retrn(Args&&... args){
		if(done){
			throw std::logic_error("Return already sent");
		}
		if(request->needs_return){
			auto ret = std::make_shared<Ret>(std::forward<Args>(args)...);
			send_return(ret);
		}
		done = true;
	}

	void err(std::shared_ptr<data::Exception> error){
		if(done){
			throw std::logic_error("Return already sent");
		}
		done = true;
		if(request->needs_return){
			send_return(error);
		}
	}

	void err(const std::string &message){
		err(std::make_shared<data::Exception>(message));
	}

	void err(const std::exception &error){
		err(error.what());
	}

	~ReturnMe(){
		if(!done){
			try{
				err("Request dropped");
			}catch(const std::exception &/*err*/){
				// no exceptions in destructors
			}
		}
	}

private:
	std::shared_ptr<const data::Request> request;
	bool done = false;

	void send_return(std::shared_ptr<data::Return> ret){
		ret->message_id = request->message_id;
		ret->source_address = request->source_address;
		ret->destination_address = request->destination_address;

		pipe::PipeIn destination = pipe::Pipe::get(ret->source_address);
		destination.push(ret);
	}
};



}
}
