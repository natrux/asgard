#include <asgard/topic/TopicPtr.h>


namespace asgard{
namespace topic{


std::mutex TopicPtr::mutex_topic_map;
std::map<std::string, std::shared_ptr<Topic>> TopicPtr::topic_map;


TopicPtr::TopicPtr():
	TopicPtr(Topic::no_name)
{
}


TopicPtr::TopicPtr(const std::string &topic_name){
	*this = topic_name;
}


TopicPtr::TopicPtr(const std::nullptr_t &):
	TopicPtr(Topic::null_name)
{
}


TopicPtr::TopicPtr(const char *topic_name):
	TopicPtr(std::string(topic_name))
{
}


TopicPtr &TopicPtr::operator=(const std::string &topic_name){
	std::lock_guard<std::mutex> lock(mutex_topic_map);
	const std::string topic_name_ = topic_name.empty() ? Topic::empty_name : topic_name;
	auto &t = topic_map[topic_name_];
	if(!t){
		t = Topic::create(topic_name_);
	}
	topic = t;
	return *this;
}


TopicPtr &TopicPtr::operator=(const std::nullptr_t &){
	return (*this = Topic::null_name);
}


std::shared_ptr<Topic> TopicPtr::operator->(){
	return topic;
}


bool TopicPtr::Compare::operator()(const TopicPtr &a, const TopicPtr &b) const{
	return is_less(a.topic, b.topic);
}



}
}


