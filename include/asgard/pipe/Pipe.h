#pragma once

#include <asgard/core/ID.h>
#include <asgard/pipe/PipeIn.h>
#include <asgard/data/Message.hxx>
#include <asgard/util/UniqueQueue.h>

#include <map>
#include <queue>
#include <mutex>
#include <condition_variable>

namespace asgard{
namespace pipe{


class Pipe{
public:
	static std::shared_ptr<Pipe> create(size_t max_size=0);
	static void bind(const core::ID &id, std::shared_ptr<Pipe> pipe);
	static void unbind(const core::ID &id, std::shared_ptr<Pipe> pipe);
	static PipeIn get(const core::ID &id);

	void push(size_t from, std::shared_ptr<const data::Message> value);
	template<class Rep, class Period>
	std::shared_ptr<const data::Message> pop(const std::chrono::duration<Rep, Period> &wait_time);
	std::shared_ptr<const data::Message> pop();
	size_t add_id();
	void close();
	bool is_open() const;

private:
	static std::mutex mutex_pipe_map;
	static std::map<core::ID, std::shared_ptr<Pipe>> pipe_map;

	mutable std::mutex mutex;
	std::condition_variable cv;
	size_t id_counter = 0;
	size_t max_size;
	bool opened = true;
	std::map<size_t, std::queue<std::shared_ptr<const data::Message>>> data;
	util::UniqueQueue<size_t> pending;
};


template<class Rep, class Period>
std::shared_ptr<const data::Message> Pipe::pop(const std::chrono::duration<Rep, Period> &wait_time){
	std::unique_lock<std::mutex> lock(mutex);

	unsigned long next = 0;
	auto found = data.end();
	while(found == data.end()){
		auto remaining_time = wait_time;
		while(opened && pending.empty() && remaining_time > std::chrono::duration<Rep, Period>::zero()){
			const auto now = std::chrono::steady_clock::now();
			cv.wait_for(lock, remaining_time);
			const auto elapsed = std::chrono::steady_clock::now() - now;
			remaining_time -= std::chrono::duration_cast<std::chrono::duration<Rep, Period>>(elapsed);
		}

		if(!opened){
			throw std::underflow_error("EOF");
		}
		if(pending.empty()){
			return nullptr;
		}

		next = pending.pop();
		found = data.find(next);
	}

	auto &queue = found->second;
	std::shared_ptr<const data::Message> result = queue.front();
	queue.pop();
	if(!queue.empty()){
		pending.push(next);
	}
	return result;
}



}
}
