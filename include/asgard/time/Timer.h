#pragma once

#include <chrono>
#include <functional>


namespace asgard{
namespace time{


template<class duration_t, class clock_t=std::chrono::steady_clock>
class Timer{
public:
	Timer(const duration_t &period_, const std::function<void()> &function_, bool periodic_):
		period(period_),
		function(function_),
		periodic(periodic_)
	{
		reset();
	}

	void reset(){
		next_time = clock_t::now() + period;
	}

	duration_t remaining() const{
		auto now = clock_t::now();
		if(next_time <= now){
			return duration_t::zero();
		}
		return std::chrono::duration_cast<duration_t>(next_time - now);
	}

	bool is_expired() const{
		return (next_time <= clock_t::now());
	}

	bool is_periodic() const{
		return periodic;
	}

	void execute() const{
		function();
	}

	struct Compare{
		bool operator()(const Timer &a, const Timer &b) const{
			return (a.next_time < b.next_time);
		}
		bool operator()(std::shared_ptr<const Timer> a, std::shared_ptr<const Timer> b) const{
			return (*this)(*a, *b);
		}
	};

private:
	duration_t period;
	std::chrono::time_point<clock_t> next_time;
	std::function<void()> function;
	bool periodic;
};


}
}


