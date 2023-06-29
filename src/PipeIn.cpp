#include <asgard/pipe/PipeIn.h>
#include <asgard/pipe/Pipe.h>


namespace asgard{
namespace pipe{



PipeIn::PipeIn(std::shared_ptr<Pipe> pipe_):
	pipe(pipe_),
	id(pipe_->add_id())
{
}


PipeIn &PipeIn::operator=(PipeIn &&other){
	if(this == &other){
		return *this;
	}

	pipe = std::move(other.pipe);
	id = std::move(other.id);
	return *this;
}


bool PipeIn::is_connected() const{
	return (pipe != nullptr);
}


PipeIn PipeIn::copy() const{
	return PipeIn(pipe);
}


void PipeIn::push(std::shared_ptr<const data::Message> value) const{
	if(pipe){
		pipe->push(id, value);
	}else{
		throw std::logic_error("No pipe connected");
	}
}


bool PipeIn::Compare::operator()(const PipeIn &a, const PipeIn &b) const{
	return std::less<const std::shared_ptr<Pipe> &>{}(a.pipe, b.pipe);
}


}
}
