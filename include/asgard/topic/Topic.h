#pragma once

#include <asgard/pipe/PipeIn.h>
#include <asgard/data/Data.hxx>
#include <asgard/data/Sample.hxx>

#include <set>
#include <queue>
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

	std::string get_name() const;
	size_t get_num_samples() const;
	size_t get_num_subscribers() const;
	std::shared_ptr<const data::Sample> get_last_sample() const;
	float get_publish_statistic() const;
	void publish(std::shared_ptr<const data::Data> value);
	void subscribe(const pipe::PipeIn &pipe_in);
	void subscribe(pipe::PipeIn &&pipe_in);
	void unsubscribe(const pipe::PipeIn &pipe_in);

private:
	mutable std::mutex mutex;
	std::string name;
	std::set<pipe::PipeIn, pipe::PipeIn::Compare> subscribers;
	size_t sample_count = 0;
	std::shared_ptr<const data::Sample> last_sample;
	std::queue<time::time> publish_times;
	const bool allow_publish;
	const bool allow_subscribe;
	time::duration statistic_period = std::chrono::seconds(10);

	Topic(const std::string &name, bool allow_publish, bool allow_subscribe);
};



}
}



