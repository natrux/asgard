#pragma once


#include <asgard/topic/Topic.h>

#include <string>
#include <map>
#include <memory>
#include <mutex>


namespace asgard{
namespace topic{


class TopicPtr{
public:
	TopicPtr();
	TopicPtr(const std::string &topic_name);
	TopicPtr(const char *topic_name);
	TopicPtr &operator=(const std::string &topic_name);
	std::shared_ptr<Topic> operator->();

	struct Compare{
		std::less<const std::shared_ptr<Topic> &> is_less;
		bool operator()(const TopicPtr &a, const TopicPtr &b) const;
	};

private:
	static std::mutex mutex_topic_map;
	static std::map<std::string, std::shared_ptr<Topic>> topic_map;

	std::shared_ptr<Topic> topic;
};



}
}




