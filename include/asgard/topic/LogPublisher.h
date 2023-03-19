#pragma once

#include <asgard/topic/TopicPtr.h>
#include <asgard/data/log_level_e.hxx>

#include <sstream>


namespace asgard{
namespace topic{


class LogPublisher : public std::stringstream{
public:
	LogPublisher(const data::log_level_e &level_, const std::string &unit_, TopicPtr topic_);
	LogPublisher(LogPublisher &&other);
	~LogPublisher();

private:
	data::log_level_e level;
	std::string unit;
	TopicPtr topic;
};


}
}
