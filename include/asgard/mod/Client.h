#pragma once

#include <asgard/mod/Messager.h>


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
	void request_async(Args&&... args){
		auto req = std::make_shared<R>(std::forward<Args>(args)...);
		request_async(req);
	}
	void request_async(std::shared_ptr<data::Request> req);
	template<class R, class... Args>
	std::shared_ptr<const data::Return> request_sync(Args&&... args){
		auto req = std::make_shared<R>(std::forward<Args>(args)...);
		return request_sync(req);
	}
	std::shared_ptr<const data::Return> request_sync(std::shared_ptr<data::Request> req);
	void send_request(std::shared_ptr<data::Request> req);

private:
	size_t num_requests = 0;
	core::ID dst_address;
	pipe::PipeIn destination;

	std::shared_ptr<const data::Request> current_request;
	std::shared_ptr<const data::Return> current_return;
};


}
}


