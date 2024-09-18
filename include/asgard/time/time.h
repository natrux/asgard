#pragma once

#include <chrono>

using namespace std::chrono_literals;


namespace asgard{
namespace time{


using clock = std::chrono::steady_clock;
using wall_clock = std::chrono::system_clock;

using time = clock::time_point;
using wall_time = wall_clock::time_point;

using duration = std::common_type<
	std::chrono::nanoseconds,
	std::chrono::microseconds,
	std::chrono::milliseconds,
	std::chrono::seconds,
	std::chrono::minutes,
	std::chrono::hours
>::type;


inline time now(){
	return clock::now();
}

inline wall_time now_wall(){
	return wall_clock::now();
}

inline duration since(const time &before){
	return now() - before;
}

inline duration since(const wall_time &before){
	return now_wall() - before;
}

inline duration until(const time &after){
	return after - now();
}

inline duration until(const wall_time &after){
	return after - now_wall();
}

inline duration immediate(){
	return duration::zero();
}

inline duration tick(){
	return since(now());
}


}
}
