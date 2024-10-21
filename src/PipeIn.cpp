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
}


PipeIn::PipeIn(const core::ID &address_, std::shared_ptr<Pipe> pipe_):
	address(address_),
	pipe(pipe_),
	id(pipe_->add_id())
{
}


PipeIn::PipeIn(PipeIn &&other){
	*this = std::move(other);
}


PipeIn &PipeIn::operator=(PipeIn &&other){
	address = std::move(other.address);
	other.address.reset();
	pipe = std::move(other.pipe);
	id = std::move(other.id);
	return *this;
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
	auto pipe_ptr = pipe.lock();
	if(address){
		const auto address_value = address.value();
		if(pipe_ptr){
			return PipeIn(address_value, pipe_ptr);
		}
		return PipeIn(address_value);
	}
	if(pipe_ptr){
		return PipeIn(pipe_ptr);
	}
	return PipeIn();
}


void PipeIn::push(std::shared_ptr<const data::Message> value){
	auto pipe_ptr = connect();
	pipe_ptr->push(id, value);
}


std::shared_ptr<Pipe> PipeIn::connect(){
	if(auto pipe_ptr = pipe.lock()){
		return pipe_ptr;
	}
	if(!address){
		throw std::logic_error("No valid pipe address");
	}
	*this = Pipe::get(address.value());
	if(auto pipe_ptr = pipe.lock()){
		return pipe_ptr;
	}
	throw std::logic_error("No pipe connected");
}


}
}
