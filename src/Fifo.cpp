#include <asgard/util/Fifo.h>

#include <stdexcept>



namespace asgard{
namespace util{


template<class Iter>
static Iter advanced(Iter &&iter, size_t count){
	std::advance(iter, count);
	return iter;
}


size_t Fifo::size() const{
	std::lock_guard<std::mutex> lock(mutex);
	return buffer.size();
}


bool Fifo::empty() const{
	std::lock_guard<std::mutex> lock(mutex);
	return buffer.empty();
}


void Fifo::push(const std::vector<uint8_t> &data){
	std::lock_guard<std::mutex> lock(mutex);
	buffer.insert(buffer.end(), data.begin(), data.end());
	condition.notify_all();
}


void Fifo::pop(std::vector<uint8_t> &data, size_t count){
	std::unique_lock<std::mutex> lock(mutex);
	while(!closed && buffer.empty()){
		condition.wait(lock);
	}
	if(closed){
		return;
	}
	const auto end = advanced(buffer.begin(), std::min(count, buffer.size()));
	data.insert(data.end(), buffer.begin(), end);
	buffer.erase(buffer.begin(), end);
}


void Fifo::close(){
	std::lock_guard<std::mutex> lock(mutex);
	closed = true;
	buffer.clear();
	condition.notify_all();
}


}
}
