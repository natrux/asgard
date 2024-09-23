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


void Fifo::push(const uint8_t *data, size_t length){
	std::lock_guard<std::mutex> lock(mutex);
	buffer.insert(buffer.end(), data, data+length);
	condition.notify_all();
}


std::vector<uint8_t> Fifo::pop(size_t count){
	std::vector<uint8_t> result;
	if(count <= 0){
		return result;
	}
	std::unique_lock<std::mutex> lock(mutex);
	while(!closed && buffer.empty()){
		condition.wait(lock);
	}
	if(closed){
		return result;
	}

	const auto count_ = std::min(count, buffer.size());
	result.resize(count_);
	const auto end = advanced(buffer.begin(), count_);
	result.insert(result.end(), buffer.begin(), end);
	buffer.erase(buffer.begin(), end);
	return result;
}


void Fifo::close(){
	std::lock_guard<std::mutex> lock(mutex);
	closed = true;
	buffer.clear();
	condition.notify_all();
}


}
}
