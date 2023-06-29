#include <asgard/mod/Module.h>

#include <thread>
#ifdef _WIN32
#include <processthreadsapi.h>
#endif


namespace asgard{
namespace mod{


std::mutex Module::mutex_started_modules;
std::condition_variable Module::cv_started_modules;
std::set<core::ID> Module::started_modules;
data::log_level_e Module::DEBUG = data::log_level_e::DEBUG;
data::log_level_e Module::INFO = data::log_level_e::INFO;
data::log_level_e Module::WARN = data::log_level_e::WARN;
data::log_level_e Module::ERROR = data::log_level_e::ERROR;


void Module::start_module(std::unique_ptr<Module> module){
	{
		std::lock_guard<std::mutex> lock(mutex_started_modules);
		started_modules.insert(module->get_id());
	}
	module->start(std::move(module));
}


void Module::shutdown_all(){
	topic::TopicPtr t("asgard.module.shutdown");
	auto shutdown = std::make_shared<data::PleaseShutDown>();
	t->publish(shutdown);
}


void Module::wait_for_shutdown(){
	std::unique_lock<std::mutex> lock(mutex_started_modules);
	while(!started_modules.empty()){
		cv_started_modules.wait(lock);
	}
}


Module::Module(const std::string &name_):
	Subscriber(name_),
	name(name_)
{
}


void Module::start(std::unique_ptr<Module> self_ptr){
	self = std::move(self_ptr);
	const std::string thread_name = get_name().substr(0, 15);
	std::thread thread(&Module::module_thread, this);
	pthread_setname_np(thread.native_handle(), thread_name.c_str());
	thread.detach();
}


void Module::main(){
	while(node_should_run()){
		if(!timers.empty() && (*timers.begin())->is_expired()){
			auto expired_timer = *timers.begin();
			timers.erase(timers.begin());
			if(expired_timer->is_periodic()){
				expired_timer->reset();
				timers.insert(expired_timer);
			}
			try{
				expired_timer->execute();
			}catch(const std::exception &err){
				log(WARN) << err.what();
			}
		}

		try{
			if(timers.empty()){
				process_next();
			}else{
				auto timeout = (*timers.begin())->remaining();
				process_next(timeout);
			}
		}catch(const std::exception &err){
			log(WARN) << err.what();
		}
	}
}


void Module::process(std::shared_ptr<const data::Request> /* request */){
}


void Module::process(std::shared_ptr<const data::Return> /* retrn */){
}


void Module::process(std::shared_ptr<const data::Sample> sample){
	auto data = sample->data;
	if(auto d = std::dynamic_pointer_cast<const data::PleaseShutDown>(data)){
		process(sample, d);
	}else{
		Messager::process(sample);
	}
}


void Module::process(std::shared_ptr<const data::Sample> /*sample*/, std::shared_ptr<const data::PleaseShutDown> data){
	process(data);
}


void Module::process(std::shared_ptr<const data::PleaseShutDown> /* value */){
	node_exit();
}


std::string Module::get_name() const{
	return name;
}


topic::LogPublisher Module::log(const data::log_level_e &level) const{
	return topic::LogPublisher(level, get_name(), output_log);
}


void Module::remove_timer(std::shared_ptr<const timer_t> timer){
	timers.erase(std::const_pointer_cast<timer_t>(timer));
}


void Module::reset_timer(std::shared_ptr<const timer_t> timer){
	auto find = timers.find(std::const_pointer_cast<timer_t>(timer));
	if(find == timers.end()){
		throw std::logic_error("timer not found");
	}
	auto mut_timer = *find;
	timers.erase(find);
	mut_timer->reset();
	timers.insert(mut_timer);
}


void Module::module_thread(){
	bool error = false;
	subscribe(input_shutdown);
	try{
		log(DEBUG) << "Init";
		init();
	}catch(const std::exception &err){
		log(ERROR) << err.what();
		error = true;
	}
	if(!error){
		try{
			log(DEBUG) << "Start of Main";
			main();
			log(DEBUG) << "End of Main";
		}catch(const std::exception &err){
			log(ERROR) << err.what();
			error = true;
		}
	}

	unsubscribe_all();
	{
		std::lock_guard<std::mutex> lock(mutex_started_modules);
		started_modules.erase(get_id());
		cv_started_modules.notify_all();
	}
	log(DEBUG) << "Exit";
	self = nullptr;
}


}
}

