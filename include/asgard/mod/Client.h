#pragma once

#include <asgard/mod/Messager.h>
#include <asgard/data/Exception.hxx>
#include <asgard/core/exception.h>

#include <future>


namespace asgard{
namespace mod{


class Client : public Messager{
public:
	Client(const core::ID &destination);
	Client(const std::string &destination);
	Client(pipe::PipeIn &&destination);

protected:
	void process(std::shared_ptr<const data::Return> ret) override;

	template<class R, class... Args>
	std::future<std::shared_ptr<const data::Return>> call(Args&&... args){
		auto req = std::make_shared<R>(std::forward<Args>(args)...);
		return call(req);
	}
	std::future<std::shared_ptr<const data::Return>> call(std::shared_ptr<data::Request> req);
	template<class R, class... Args>
	std::shared_ptr<const data::Return> request(Args&&... args){
		auto req = std::make_shared<R>(std::forward<Args>(args)...);
		return request(req);
	}
	std::shared_ptr<const data::Return> request(std::shared_ptr<data::Request> req);
	void send_request(std::shared_ptr<data::Request> req);

	size_t last_request_id() const;
	virtual void on_return_received(std::shared_ptr<const data::Return> /*ret*/){ /* default empty */ };


	template<class Return, class Ret>
	void fulfill_promise(std::promise<Ret> &promise, std::shared_ptr<const Return> ret){
		promise.set_value(ret->result);
	}
	template<class Return>
	void fulfill_promise(std::promise<void> &promise, std::shared_ptr<const Return> /*ret*/){
		promise.set_value();
	}

	template<class Return, class Ret>
	void return_dispatch_helper(std::shared_ptr<const data::Return> ret, std::map<size_t, std::promise<Ret>> &pending){
		const auto find = pending.find(ret->message_id);
		if(find != pending.end()){
			if(auto ex = std::dynamic_pointer_cast<const data::Exception>(ret)){
				find->second.set_exception(std::make_exception_ptr(core::exception(ex)));
			}else if(auto ret_ = std::dynamic_pointer_cast<const Return>(ret)){
				fulfill_promise(find->second, ret_);
			}
			pending.erase(find);
		}
	}

private:
	size_t num_requests = 0;
	core::ID dst_address;
	pipe::PipeIn destination;

	std::map<size_t, std::promise<std::shared_ptr<const data::Return>>> pending_requests;
};


}
}


