#pragma once

#include <deque>
#include <vector>
#include <mutex>
#include <condition_variable>


namespace asgard{
namespace util{


class Fifo{
public:
	size_t size() const;
	bool empty() const;
	void push(const std::vector<uint8_t> &data);
	void push(const uint8_t *data, size_t length);
	std::vector<uint8_t> pop(size_t count);
	void close();

private:
	mutable std::mutex mutex;
	std::condition_variable condition;
	std::deque<uint8_t> buffer;
	bool closed = false;
};


}
}
