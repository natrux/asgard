#include <asgard/topic/LogPublisher.h>
#include <asgard/data/LogMessage.hxx>


namespace asgard{
namespace topic{


LogPublisher::LogPublisher(const data::log_level_e &level_, const std::string &unit_, TopicPtr topic_):
	std::stringstream(),
	level(level_),
	unit(unit_),
	topic(topic_)
{
}


LogPublisher::LogPublisher(LogPublisher &&other):
	std::stringstream(std::move(other)),
	level(std::move(other.level)),
	unit(std::move(other.unit)),
	topic(std::move(other.topic))
{
}


LogPublisher::~LogPublisher(){
	auto msg = std::make_shared<data::LogMessage>();
	msg->time = time::clock::now();
	msg->level = level;
	msg->unit = unit;
	msg->message = str();
	topic->publish(msg);
}




}
}


