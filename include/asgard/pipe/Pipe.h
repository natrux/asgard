#pragma once

#include <asgard/core/ID.h>
#include <asgard/time/time.h>
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
	static void rebind(const core::ID &id, std::shared_ptr<Pipe> old_pipe, std::shared_ptr<Pipe> new_pipe);
	static PipeIn get(const core::ID &id);

	void push(size_t from, std::shared_ptr<const data::Message> value);
	std::shared_ptr<const data::Message> pop(const time::duration &wait_time);
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


}
}
