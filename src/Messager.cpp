#include <asgard/mod/Messager.h>


namespace asgard{
namespace mod{


Messager::Messager():
	id()
{
	pipe_in = pipe::Pipe::create();
}


Messager::Messager(const core::ID &id_):
	id(id_)
{
	pipe_in = pipe::Pipe::create();
}


core::ID Messager::get_id() const{
	return id;
}


bool Messager::node_should_run() const{
	std::lock_guard<std::mutex> lock(mutex_should_run);
	return should_run;
}


void Messager::node_exit(){
	{
		std::lock_guard<std::mutex> lock(mutex_should_run);
		should_run = false;
	}
	close();
}


pipe::PipeIn Messager::make_pipe_in() const{
	return pipe::PipeIn(pipe_in);
}


void Messager::bind(const core::ID &id_) const{
	pipe::Pipe::bind(id_, pipe_in);
}


void Messager::bind(const std::string &name) const{
	bind(core::ID(name));
}


void Messager::unbind(const core::ID &id_) const{
	pipe::Pipe::unbind(id_, pipe_in);
}


void Messager::bind_other(std::shared_ptr<const Messager> other) const{
	const core::ID other_id = other->get_id();
	bind(other_id);
}


void Messager::unbind_other(std::shared_ptr<const Messager> other) const{
	const core::ID other_id = other->get_id();
	unbind(other_id);
}


std::shared_ptr<const data::Data> Messager::get_next(){
	try{
		return pipe_in->pop();
	}catch(const std::underflow_error &err){
		// pipe closed
		node_exit();
	}
	return nullptr;
}


void Messager::process_next(){
	auto data = get_next();
	if(data){
		process(data);
	}
}


void Messager::process(std::shared_ptr<const data::Data> data){
	if(auto value = std::dynamic_pointer_cast<const data::Value>(data)){
		process(value);
	}else if(auto request = std::dynamic_pointer_cast<const data::Request>(data)){
		process(request);
	}else if(auto retrn = std::dynamic_pointer_cast<const data::Return>(data)){
		process(retrn);
	}
}


void Messager::close(){
	pipe_in->close();
}


}
}
