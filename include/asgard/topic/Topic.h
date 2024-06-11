#pragma once

#include <asgard/pipe/PipeIn.h>
#include <asgard/data/Data.hxx>

#include <set>
#include <memory>
#include <mutex>


namespace asgard{
namespace topic{


class Topic{
public:
	const static std::string no_name;
	const static std::string empty_name;
	const static std::string null_name;
	static std::shared_ptr<Topic> create(const std::string &name);

	void publish(std::shared_ptr<const data::Data> value);
	void subscribe(const pipe::PipeIn &pipe_in);
	void subscribe(pipe::PipeIn &&pipe_in);
	void unsubscribe(const pipe::PipeIn &pipe_in);

private:
	std::mutex mutex;
	std::set<pipe::PipeIn, pipe::PipeIn::Compare> subscribers;
	size_t sample_count = 0;
	const bool allow_publish;
	const bool allow_subscribe;

	Topic(bool allow_publish, bool allow_subscribe);
};



}
}



