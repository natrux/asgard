#include <asgard/pipe/Pipe.h>

#include <stdexcept>


namespace asgard{
namespace pipe{


std::mutex Pipe::mutex_pipe_map;
std::map<core::ID, std::shared_ptr<Pipe>> Pipe::pipe_map;


std::shared_ptr<Pipe> Pipe::create(size_t max_size){
	auto pipe = std::make_shared<Pipe>();
	pipe->max_size = max_size;
	return pipe;
}


void Pipe::bind(const core::ID &id, std::shared_ptr<Pipe> pipe){
	std::lock_guard<std::mutex> lock(mutex_pipe_map);
	{
		auto find = pipe_map.find(id);
		if(find != pipe_map.end()){
			if(find->second == pipe){
				// Nothing to do
				return;
			}else{
				throw std::logic_error("There is already a pipe at address " + id.str());
			}
		}
	}

	pipe_map[id] = pipe;
}


void Pipe::unbind(const core::ID &id, std::shared_ptr<Pipe> pipe){
	std::lock_guard<std::mutex> lock(mutex_pipe_map);
	auto find = pipe_map.find(id);
	if(find == pipe_map.end()){
		throw std::logic_error("No pipe at address " + id.str());
	}else if(find->second != pipe){
		throw std::logic_error("Wrong pipe found at address " + id.str());
	}
	pipe_map.erase(find);
}


PipeIn Pipe::get(const core::ID &id){
	std::lock_guard<std::mutex> lock(mutex_pipe_map);
	auto find = pipe_map.find(id);
	if(find != pipe_map.end()){
		return PipeIn(find->second);
	}
	throw std::logic_error("No pipe at address " + id.str());
}


void Pipe::push(size_t from, std::shared_ptr<const data::Message> value){
	std::lock_guard<std::mutex> lock(mutex);
	if(!opened){
		throw std::runtime_error("Pipe closed");
	}
	auto &queue = data[from];
	if(max_size > 0 && queue.size() >= max_size){
		throw std::overflow_error("Pipe input full");
	}
	queue.push(value);
	pending.push(from);
	cv.notify_all();
}


std::shared_ptr<const data::Message> Pipe::pop(){
	std::unique_lock<std::mutex> lock(mutex);

	unsigned long next = 0;
	auto found = data.end();
	while(found == data.end()){
		while(opened && pending.empty()){
			cv.wait(lock);
		}

		if(!opened){
			throw std::underflow_error("EOF");
		}

		next = pending.pop();
		found = data.find(next);
	}

	auto &queue = found->second;
	auto result = queue.front();
	queue.pop();
	if(!queue.empty()){
		pending.push(next);
	}
	return result;

	// no good, overflow:
	//return pop(std::chrono::seconds::max());
}


size_t Pipe::add_id(){
	std::lock_guard<std::mutex> lock(mutex);
	return id_counter++;
}


void Pipe::close(){
	std::lock_guard<std::mutex> lock(mutex);
	if(opened){
		opened = false;
		cv.notify_all();
	}
}


bool Pipe::is_open() const{
	std::lock_guard<std::mutex> lock(mutex);
	return opened;
}


}
}

