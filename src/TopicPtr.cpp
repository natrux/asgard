#include <asgard/topic/TopicPtr.h>


namespace asgard{
namespace topic{


std::mutex TopicPtr::mutex_topic_map;
std::map<std::string, std::shared_ptr<Topic>> TopicPtr::topic_map;


TopicPtr::TopicPtr():
	TopicPtr("asgard.lost_and_found")
{
}


TopicPtr::TopicPtr(const std::string &topic_name){
	*this = topic_name;
}


TopicPtr::TopicPtr(const char *topic_name):
	TopicPtr(std::string(topic_name))
{
}


TopicPtr &TopicPtr::operator=(const std::string &topic_name){
	std::lock_guard<std::mutex> lock(mutex_topic_map);
	auto &t = topic_map[topic_name];
	if(!t){
		t = std::make_shared<Topic>();
	}
	topic = t;
	return *this;
}


std::shared_ptr<Topic> TopicPtr::operator->(){
	return topic;
}


bool TopicPtr::Compare::operator()(const TopicPtr &a, const TopicPtr &b) const{
	return is_less(a.topic, b.topic);
}



}
}


