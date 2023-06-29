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
	void publish(std::shared_ptr<const data::Data> value);
	void subscribe(const pipe::PipeIn &pipe_in);
	void subscribe(pipe::PipeIn &&pipe_in);
	void unsubscribe(const pipe::PipeIn &pipe_in);

private:
	std::mutex mutex;
	std::set<pipe::PipeIn, pipe::PipeIn::Compare> subscribers;
	size_t sample_count = 0;
};



}
}



