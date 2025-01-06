#include <asgard/time/strtime.h>


namespace asgard{
namespace time{


template<class T>
static void helper(duration &period, const std::string &name, std::string &result, size_t &count){
	const auto tm = std::chrono::duration_cast<T>(period);
	if(tm != tm.zero()){
		if(!result.empty()){
			result += " ";
		}
		result += std::to_string(tm.count()) + name;
		count++;
		period -= tm;
	}else if(count > 0){
		count++;
	}
}


std::string strtime(const duration &period, size_t terms){
	std::string result;
	size_t count = 0;
	auto period_ = period;

	helper<std::chrono::hours>(period_, "h", result, count);
	if(count >= terms) return result;
	helper<std::chrono::minutes>(period_, "m", result, count);
	if(count >= terms) return result;
	helper<std::chrono::seconds>(period_, "s", result, count);
	if(count >= terms) return result;
	helper<std::chrono::milliseconds>(period_, "ms", result, count);
	if(count >= terms) return result;
	helper<std::chrono::microseconds>(period_, "us", result, count);
	if(count >= terms) return result;
	helper<std::chrono::nanoseconds>(period_, "ns", result, count);
	if(count >= terms) return result;

	if(result.empty()){
		result = "0s";
	}
	return result;
}


template<class T>
static std::string strtime(const T &t, const T &now, size_t terms){
	if(t > now){
		return "in " + strtime(t - now, terms);
	}else if(t < now){
		return strtime(now - t, terms) + " ago";
	}
	return "now";
}


std::string strtime(const time &t, size_t terms){
	return strtime(t, now(), terms);
}


std::string strtime(const wall_time &t, size_t terms){
	return strtime(t, now_wall(), terms);
}


}
}

