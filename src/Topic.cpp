#include <asgard/topic/Topic.h>


namespace asgard{
namespace topic{


void Topic::publish(std::shared_ptr<const data::Data> value){
	std::lock_guard<std::mutex> lock(mutex);
	for(auto iter=subscribers.begin(); iter!=subscribers.end(); /* no iter */){
		bool itered = false;
		try{
			(*iter).push(value);
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
	std::lock_guard<std::mutex> lock(mutex);
	subscribers.insert(std::move(pipe_in));
}


void Topic::unsubscribe(const pipe::PipeIn &pipe_in){
	std::lock_guard<std::mutex> lock(mutex);
	subscribers.erase(pipe_in);
}


}
}


