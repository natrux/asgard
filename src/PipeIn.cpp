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


bool PipeIn::operator==(const PipeIn &other) const{
	if(pipe || other.pipe){
		return pipe == other.pipe;
	}
	return address == other.address;
}


bool PipeIn::is_connected() const{
	return (pipe != nullptr);
}


PipeIn PipeIn::copy() const{
	if(pipe){
		return PipeIn(pipe);
	}
	return PipeIn(address);
}


void PipeIn::push(std::shared_ptr<const data::Message> value){
	if(!pipe){
		connect();
	}
	if(!pipe){
		throw std::logic_error("No pipe connected");
	}
	pipe->push(id, value);
}


void PipeIn::connect(){
	*this = Pipe::get(address);
}


}
}
