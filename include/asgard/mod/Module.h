#pragma once

#include <asgard/mod/Subscriber.h>
#include <asgard/topic/TopicPtr.h>
#include <asgard/pipe/Pipe.h>
#include <asgard/time/Timer.h>
#include <asgard/time/time.h>
#include <asgard/data/PleaseShutDown.hxx>
#include <asgard/data/log_level_e.hxx>
#include <asgard/topic/LogPublisher.h>

#include <memory>
#include <mutex>
#include <condition_variable>
#include <map>
#include <set>
#include <future>


namespace asgard{
namespace mod{


class Module : public Subscriber{
	using timer_t = time::Timer<time::duration, time::clock>;

public:
	Module(const std::string &name);

	static void start_module(std::unique_ptr<Module> module);
	template<class T, class... Args> static void start_module(Args&&... args);
	static void shutdown_all();
	static void wait_for_shutdown();

protected:
	static const data::log_level_e DEBUG;
	static const data::log_level_e INFO;
	static const data::log_level_e WARN;
	static const data::log_level_e ERROR;

	virtual void init(){ /* default empty */ };
	virtual void main();

	std::string get_name() const;
	topic::LogPublisher log(const data::log_level_e &level) const;
	std::shared_ptr<const timer_t> set_timer(const time::duration &period, const std::function<void()> &function, bool periodic=true);
	std::shared_ptr<const timer_t> set_timeout(const time::duration &period, const std::function<void()> &function);
	void remove_timer(std::shared_ptr<const timer_t> timer);
	void reset_timer(std::shared_ptr<const timer_t> timer);
	void add_task(const std::function<void()> &function);
	void add_pending_request(std::shared_ptr<const data::Request> request, std::future<std::shared_ptr<const data::Return>> &&future);

	void answer_pending_requests();
	void execute_timers();
	void receive_messages();

	void process(std::shared_ptr<const data::Request> request) override;
	void process(std::shared_ptr<const data::Return> retrn) override;
	void process(std::shared_ptr<const data::Sample> sample) override;

	using Messager::process;
	void process(std::shared_ptr<const data::Sample> sample, std::shared_ptr<const data::PleaseShutDown> data);
	void process(std::shared_ptr<const data::PleaseShutDown> data);

private:
	static std::mutex mutex_started_modules;
	static std::condition_variable cv_started_modules;
	static std::set<core::ID> started_modules;

	std::string name;
	std::unique_ptr<Module> self;
	std::set<std::shared_ptr<timer_t>, timer_t::Compare> timers;
	std::map<std::shared_ptr<const data::Request>, std::future<std::shared_ptr<const data::Return>>> pending_requests;

	topic::TopicPtr input_shutdown = "asgard.module.shutdown";
	topic::TopicPtr output_log = "asgard.log";

	void start(std::unique_ptr<Module> self_ptr);
	void module_thread();
};


template<class T, class... Args>
void Module::start_module(Args&&... args){
	auto mod = std::make_unique<T>(std::forward<Args>(args)...);
	start_module(std::move(mod));
}



}
}

