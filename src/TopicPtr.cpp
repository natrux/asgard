#include <asgard/topic/TopicPtr.h>


namespace asgard{
namespace topic{


std::mutex TopicPtr::mutex_topic_map;
std::map<std::string, std::weak_ptr<Topic>> TopicPtr::topic_map;


std::vector<TopicPtr> TopicPtr::get_all_topics(){
	std::lock_guard<std::mutex> lock(mutex_topic_map);
	std::vector<TopicPtr> result;
	for(const auto &entry : topic_map){
		if(auto topic = entry.second.lock()){
			result.push_back(topic);
		}
	}
	return result;
}


TopicPtr::TopicPtr():
	TopicPtr(Topic::no_name)
{
}


TopicPtr::TopicPtr(const std::string &topic_name){
	*this = topic_name;
}


TopicPtr::TopicPtr(const char *topic_name){
	if(topic_name){
		*this = std::string(topic_name);
	}else{
		*this = nullptr;
	}
}


TopicPtr::TopicPtr(const std::nullptr_t &):
	TopicPtr(Topic::null_name)
{
}


TopicPtr::~TopicPtr(){
	cleanup();
}


TopicPtr &TopicPtr::operator=(const std::string &topic_name){
	cleanup();

	std::lock_guard<std::mutex> lock(mutex_topic_map);
	const std::string topic_name_ = topic_name.empty() ? Topic::empty_name : topic_name;
	auto &w = topic_map[topic_name_];
	if(auto t = w.lock()){
		topic = t;
	}else{
		topic = Topic::create(topic_name_);
		w = topic;
	}
	return *this;
}


TopicPtr &TopicPtr::operator=(const char *topic_name){
	if(topic_name){
		return (*this = std::string(topic_name));
	}
	return (*this = nullptr);
}


TopicPtr &TopicPtr::operator=(const std::nullptr_t &){
	return (*this = Topic::null_name);
}


std::shared_ptr<Topic> TopicPtr::operator->() const{
	return topic;
}


bool TopicPtr::Compare::operator()(const TopicPtr &a, const TopicPtr &b) const{
	return is_less(a.topic, b.topic);
}


TopicPtr::TopicPtr(std::shared_ptr<Topic> topic_):
	topic(topic_)
{
}


void TopicPtr::cleanup(){
	if(!topic){
		return;
	}
	const std::string old_name = topic->get_name();
	topic = nullptr;

	std::lock_guard<std::mutex> lock(mutex_topic_map);
	const auto iter = topic_map.find(old_name);
	if(iter != topic_map.end() && !iter->second.lock()){
		// no references left, garbage collect
		topic_map.erase(iter);
	}
}



}
}


