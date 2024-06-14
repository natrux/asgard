#include <asgard/topic/Topic.h>
#include <asgard/data/Sample.hxx>

#include <stdexcept>


namespace asgard{
namespace topic{


const std::string Topic::no_name = "asgard.lost_and_found";
const std::string Topic::empty_name = "asgard.empty";
const std::string Topic::null_name = "asgard.null";


std::shared_ptr<Topic> Topic::create(const std::string &name){
	static std::shared_ptr<Topic> lost_and_found(new Topic(true, false));
	static std::shared_ptr<Topic> empty_topic(new Topic(false, true));
	static std::shared_ptr<Topic> null_topic(new Topic(false, false));

	if(name == no_name){
		return lost_and_found;
	}
	if(name == empty_name){
		return empty_topic;
	}
	if(name == null_name){
		return null_topic;
	}
	return std::shared_ptr<Topic>(new Topic(true, true));
}


void Topic::publish(std::shared_ptr<const data::Data> value){
	if(!allow_publish){
		return;
	}

	auto sample = std::make_shared<data::Sample>();
	sample->time = time::now();
	sample->data = value;

	std::lock_guard<std::mutex> lock(mutex);
	for(auto iter=subscribers.begin(); iter!=subscribers.end(); /* no iter */){
		bool itered = false;
		try{
			(*iter).push(sample);
		}catch(const std::overflow_error &err){
		}catch(const std::runtime_error &err){
			// pipe closed
			iter = subscribers.erase(iter);
			itered = true;
		}catch(const std::exception &err){
		}
		if(!itered){
			iter++;
		}
	}
	sample_count++;
}


void Topic::subscribe(const pipe::PipeIn &pipe_in){
	subscribe(pipe_in.copy());
}


void Topic::subscribe(pipe::PipeIn &&pipe_in){
	if(!allow_subscribe){
		return;
	}

	std::lock_guard<std::mutex> lock(mutex);
	subscribers.insert(std::move(pipe_in));
}


void Topic::unsubscribe(const pipe::PipeIn &pipe_in){
	std::lock_guard<std::mutex> lock(mutex);
	subscribers.erase(pipe_in);
}


Topic::Topic(bool allow_publish_, bool allow_subscribe_):
	allow_publish(allow_publish_),
	allow_subscribe(allow_subscribe_)
{
}


}
}


