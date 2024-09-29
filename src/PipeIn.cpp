#include <asgard/pipe/PipeIn.h>
#include <asgard/pipe/Pipe.h>


namespace asgard{
namespace pipe{



PipeIn::PipeIn(const core::ID &address_):
	address(address_)
{
}


PipeIn::PipeIn(std::shared_ptr<Pipe> pipe_):
	pipe(pipe_),
	id(pipe_->add_id())
{
	address.set(0);
}


bool PipeIn::operator==(const PipeIn &other) const{
	auto own_pipe = pipe.lock();
	auto other_pipe = other.pipe.lock();
	if(own_pipe || other_pipe){
		return own_pipe == other_pipe;
	}
	return address == other.address;
}


PipeIn PipeIn::copy() const{
	if(auto pipe_ptr = pipe.lock()){
		return PipeIn(pipe_ptr);
	}
	return PipeIn(address);
}


void PipeIn::push(std::shared_ptr<const data::Message> value){
	std::shared_ptr<Pipe> pipe_ptr = pipe.lock();
	if(!pipe_ptr){
		connect();
		pipe_ptr = pipe.lock();
	}
	if(!pipe_ptr){
		throw std::logic_error("No pipe connected");
	}
	pipe_ptr->push(id, value);
}


void PipeIn::connect(){
	*this = Pipe::get(address);
}


}
}
