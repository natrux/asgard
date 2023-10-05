#pragma once

#include <chrono>


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


}
}
