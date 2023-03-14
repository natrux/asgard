#include <asgard/mod/Subscriber.h>


namespace asgard{
namespace mod{


Subscriber::Subscriber():
	Messager()
{
}


Subscriber::Subscriber(const core::ID &id_):
	Messager(id_)
{
}


void Subscriber::subscribe(topic::TopicPtr topic){
	topic->subscribe(make_pipe_in());
	subscriptions.insert(topic);
}


void Subscriber::unsubscribe(topic::TopicPtr topic){
	topic->unsubscribe(make_pipe_in());
	subscriptions.erase(topic);
}


void Subscriber::unsubscribe_all(){
	auto copy = subscriptions;
	for(const auto &topic : copy){
		unsubscribe(topic);
	}
}


void Subscriber::publish(std::shared_ptr<const data::Value> value, topic::TopicPtr topic) const{
	topic->publish(value);
}


}
}





