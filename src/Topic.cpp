#include <asgard/topic/Topic.h>

#include <stdexcept>


namespace asgard{
namespace topic{


const std::string Topic::no_name = "asgard.lost_and_found";
const std::string Topic::empty_name = "asgard.empty";
const std::string Topic::null_name = "asgard.null";


std::shared_ptr<Topic> Topic::create(const std::string &name){
	static std::shared_ptr<Topic> lost_and_found(new Topic(no_name, true, false));
	static std::shared_ptr<Topic> empty_topic(new Topic(empty_name, false, true));
	static std::shared_ptr<Topic> null_topic(new Topic(null_name, false, false));

	if(name == no_name){
		return lost_and_found;
	}
	if(name == empty_name){
		return empty_topic;
	}
	if(name == null_name){
		return null_topic;
	}
	return std::shared_ptr<Topic>(new Topic(name, true, true));
}


std::string Topic::get_name() const{
	return name;
}


size_t Topic::get_num_samples() const{
	std::lock_guard<std::mutex> lock(mutex);
	return sample_count;
}


size_t Topic::get_num_subscribers() const{
	std::lock_guard<std::mutex> lock(mutex);
	return subscribers.size();
}


std::shared_ptr<const data::Sample> Topic::get_last_sample() const{
	std::lock_guard<std::mutex> lock(mutex);
	return last_sample;
}


float Topic::get_publish_statistic() const{
	std::queue<time::time> copy;
	time::duration period;
	{
		// no update when const
		std::lock_guard<std::mutex> lock(mutex);
		copy = publish_times;
		period = statistic_period;
	}
	while(!copy.empty() && time::since(copy.front()) > period){
		copy.pop();
	}
	const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(period).count();
	return static_cast<float>(copy.size()) / seconds;
}


void Topic::publish(std::shared_ptr<const data::Data> value, bool retained){
	if(!allow_publish){
		return;
	}

	auto sample = std::make_shared<data::Sample>();
	sample->time = time::now();
	sample->retained = retained;
	sample->data = value;

	std::lock_guard<std::mutex> lock(mutex);
	for(auto iter=subscribers.begin(); iter!=subscribers.end(); /* no iter */){
		bool itered = false;
		try{
			(*iter).push(sample);
		}catch(const std::overflow_error &){
		}catch(const std::runtime_error &){
			// pipe closed
			iter = subscribers.erase(iter);
			itered = true;
		}catch(const std::exception &){
		}
		if(!itered){
			iter++;
		}
	}
	sample_count++;
	last_sample = sample;
	publish_times.push(sample->time);
	while(!publish_times.empty() && time::since(publish_times.front()) > statistic_period){
		publish_times.pop();
	}
}


void Topic::subscribe(const pipe::PipeIn &pipe_in){
	subscribe(pipe_in.copy());
}


void Topic::subscribe(pipe::PipeIn &&pipe_in){
	if(!allow_subscribe){
		return;
	}

	std::lock_guard<std::mutex> lock(mutex);
	if(last_sample && last_sample->retained){
		try{
			pipe_in.push(last_sample);
		}catch(const std::exception &){
		}
	}
	subscribers.push_back(std::move(pipe_in));
}


void Topic::unsubscribe(const pipe::PipeIn &pipe_in){
	std::lock_guard<std::mutex> lock(mutex);
	for(auto iter=subscribers.begin(); iter!=subscribers.end(); iter++){
		if((*iter) == pipe_in){
			subscribers.erase(iter);
			break;
		}
	}
}


Topic::Topic(const std::string &name_, bool allow_publish_, bool allow_subscribe_):
	name(name_),
	allow_publish(allow_publish_),
	allow_subscribe(allow_subscribe_)
{
}


}
}


