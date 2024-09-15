#pragma once


#include <asgard/topic/Topic.h>

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <mutex>


namespace asgard{
namespace topic{


class TopicPtr{
public:
	static std::vector<TopicPtr> get_all_topics();

	TopicPtr();
	TopicPtr(const TopicPtr &other) = default;
	TopicPtr(TopicPtr &&other) = default;
	TopicPtr(const std::string &topic_name);
	TopicPtr(const char *topic_name);
	TopicPtr(const std::nullptr_t &);
	~TopicPtr();
	TopicPtr &operator=(const TopicPtr &other) = default;
	TopicPtr &operator=(TopicPtr &&other) = default;
	TopicPtr &operator=(const std::string &topic_name);
	TopicPtr &operator=(const char *topic_name);
	TopicPtr &operator=(const std::nullptr_t &);
	std::shared_ptr<Topic> operator->() const;

	struct Compare{
		std::less<const std::shared_ptr<Topic> &> is_less;
		bool operator()(const TopicPtr &a, const TopicPtr &b) const;
	};

private:
	static std::mutex mutex_topic_map;
	static std::map<std::string, std::weak_ptr<Topic>> topic_map;

	std::shared_ptr<Topic> topic;

	TopicPtr(std::shared_ptr<Topic> topic);
	void cleanup();
};



}
}




