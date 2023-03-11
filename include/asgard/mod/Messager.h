#pragma once

#include <asgard/pipe/Pipe.h>
#include <asgard/pipe/PipeIn.h>
#include <asgard/data/Data.h>
#include <asgard/data/Value.h>
#include <asgard/data/Request.h>
#include <asgard/data/Return.h>


namespace asgard{
namespace mod{


class Messager{
public:
	Messager();
	Messager(const core::ID &id);
	virtual ~Messager() = default;

protected:
	core::ID get_id() const;
	bool node_should_run() const;
	void node_exit();
	pipe::PipeIn make_pipe_in() const;
	void bind(const core::ID &id) const;
	void bind(const std::string &name) const;
	void unbind(const core::ID &id) const;
	void bind_other(std::shared_ptr<const Messager> other) const;
	void unbind_other(std::shared_ptr<const Messager> other) const;

	std::shared_ptr<const data::Data> get_next();
	template<class Rep, class Period>
	std::shared_ptr<const data::Data> get_next(const std::chrono::duration<Rep, Period> &timeout){
		try{
			return pipe_in->pop(timeout);
		}catch(const std::underflow_error &err){
			node_exit();
		}
		return nullptr;
	}
	void process_next();
	template<class Rep, class Period>
	void process_next(const std::chrono::duration<Rep, Period> &timeout){
		auto data = get_next(timeout);
		if(data){
			process(data);
		}
	}
	void process(std::shared_ptr<const data::Data> data);
	virtual void process(std::shared_ptr<const data::Value> /*value*/){ /* default empty */ }
	virtual void process(std::shared_ptr<const data::Request> /*request*/){ /* default empty */ }
	virtual void process(std::shared_ptr<const data::Return> /*retrn*/){ /* default empty */ }

private:
	const core::ID id;
	std::shared_ptr<pipe::Pipe> pipe_in;
	mutable std::mutex mutex_should_run;
	bool should_run = true;

	void close();
};


}
}
