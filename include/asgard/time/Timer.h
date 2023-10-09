#pragma once

#include <asgard/time/time.h>

#include <chrono>
#include <functional>


namespace asgard{
namespace time{


template<class duration_t=duration, class clock_t=clock>
class Timer{
public:
	Timer(const duration_t &period_, const std::function<void()> &function_, bool periodic_):
		period(period_),
		function(function_),
		periodic(periodic_)
	{
		reset();
	}

	void reset(const std::chrono::time_point<clock_t> &now){
		next_time = now + period;
	}

	void reset(){
		reset(clock_t::now());
	}

	duration_t remaining(const std::chrono::time_point<clock_t> &now) const{
		if(is_expired(now)){
			return duration_t::zero();
		}
		return std::chrono::duration_cast<duration_t>(next_time - now);
	}

	duration_t remaining() const{
		return remaining(clock_t::now());
	}

	bool is_expired(const std::chrono::time_point<clock_t> &now) const{
		return (next_time <= now);
	}

	bool is_expired() const{
		return is_expired(clock_t::now());
	}

	bool is_periodic() const{
		return periodic;
	}

	void execute() const{
		if(function){
			function();
		}
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


