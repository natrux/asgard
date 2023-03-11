#pragma once

#include <asgard/mod/Messager.h>
#include <asgard/topic/TopicPtr.h>
#include <asgard/data/Data.h>


namespace asgard{
namespace mod{


class Subscriber : public Messager{
public:
	Subscriber();
	Subscriber(const core::ID &id);

protected:
	void subscribe(topic::TopicPtr topic);
	void unsubscribe(topic::TopicPtr topic);
	void unsubscribe_all();
	void publish(std::shared_ptr<const data::Data> value, topic::TopicPtr topic) const;

private:
	std::set<topic::TopicPtr, topic::TopicPtr::Compare> subscriptions;
};



}
}
