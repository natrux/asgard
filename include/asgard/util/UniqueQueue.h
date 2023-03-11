#pragma once

#include <queue>
#include <set>
#include <stdexcept>

namespace asgard{
namespace util{


template<class T>
class UniqueQueue{
public:
	void push(const T &value);
	T pop();
	bool empty() const;

private:
	std::queue<T> queue;
	std::set<T> contained;
};



template<class T>
void UniqueQueue<T>::push(const T &value){
	if(contained.find(value) != contained.end()){
		return;
	}
	queue.push(value);
	contained.insert(value);
}


template<class T>
T UniqueQueue<T>::pop(){
	if(queue.empty()){
		throw std::runtime_error("Queue empty");
	}
	T result = queue.front();
	queue.pop();
	contained.erase(result);
	return result;
}


template<class T>
bool UniqueQueue<T>::empty() const{
	return queue.empty();
}


}
}
