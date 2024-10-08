#include <asgard/mod/Messager.h>
#include <asgard/data/Interrupt.hxx>


namespace asgard{
namespace mod{


Messager::Messager():
	Messager(core::ID())
{
}


Messager::Messager(const core::ID &id_):
	id(id_),
	pipe_in(pipe::Pipe::create()),
	own_input(make_pipe_in())
{
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


void Messager::bind() const{
	bind(get_id());
}


void Messager::bind(const core::ID &id_) const{
	pipe::Pipe::bind(id_, pipe_in);
}


void Messager::bind(const std::string &name) const{
	bind(core::ID(name));
}


void Messager::unbind() const{
	unbind(get_id());
}


void Messager::unbind(const core::ID &id_) const{
	pipe::Pipe::unbind(id_, pipe_in);
}


void Messager::bind_other(std::shared_ptr<Messager> other, const core::ID &other_id){
	pipe::Pipe::rebind(other_id, other->pipe_in, pipe_in);
	other_ids[other_id] = other;
}


void Messager::bind_other(std::shared_ptr<Messager> other){
	bind_other(other, other->get_id());
}


void Messager::unbind_other(std::shared_ptr<const Messager> other, const core::ID &other_id){
	pipe::Pipe::rebind(other_id, pipe_in, other->pipe_in);
	other_ids.erase(other_id);
}


void Messager::unbind_other(std::shared_ptr<const Messager> other){
	unbind_other(other, other->get_id());
}


std::shared_ptr<const data::Message> Messager::get_next(){
	try{
		return pipe_in->pop();
	}catch(const std::underflow_error &err){
		// pipe closed
		node_exit();
	}
	return nullptr;
}


std::shared_ptr<const data::Message> Messager::get_next(const time::duration &timeout){
	try{
		return pipe_in->pop(timeout);
	}catch(const std::underflow_error &err){
		node_exit();
	}
	return nullptr;
}


bool Messager::process_next(){
	auto data = get_next();
	if(data){
		process(data);
		return true;
	}
	return false;
}


bool Messager::process_next(const time::duration &timeout){
	auto data = get_next(timeout);
	if(data){
		process(data);
		return true;
	}
	return false;
}


void Messager::interrupt(){
	auto message = std::make_shared<data::Interrupt>();
	own_input.push(message);
}


void Messager::process(std::shared_ptr<const data::Message> message){
	if(auto rpc = std::dynamic_pointer_cast<const data::RPC>(message)){
		process(rpc);
	}else if(std::dynamic_pointer_cast<const data::Interrupt>(message)){
		// Nothing to do, but interrupted waiting
	}else if(auto sample = std::dynamic_pointer_cast<const data::Sample>(message)){
		process(sample);
	}
}


void Messager::process(std::shared_ptr<const data::RPC> rpc){
	if(rpc->destination_address == 0 || rpc->destination_address == get_id()){
		if(auto request = std::dynamic_pointer_cast<const data::Request>(rpc)){
			process(request);
		}else if(auto retrn = std::dynamic_pointer_cast<const data::Return>(rpc)){
			process(retrn);
		}
	}else{
		for(const auto &entry : other_ids){
			if(rpc->destination_address == entry.first){
				entry.second->process(rpc);
				break;
			}
		}
	}
}


void Messager::close(){
	pipe_in->close();
}


}
}
