#pragma once

#include <asgard/core/ID.h>
#include <asgard/data/Message.hxx>

#include <memory>

namespace asgard{
namespace pipe{


class Pipe;

class PipeIn{
public:
	PipeIn(const core::ID &address);
	PipeIn(std::shared_ptr<Pipe> pipe);
	PipeIn(const PipeIn &other) = delete;
	PipeIn(PipeIn &&other) = default;
	PipeIn &operator=(const PipeIn &other) = delete;
	PipeIn &operator=(PipeIn &&other) = default;

	bool operator==(const PipeIn &other) const;

	bool is_connected() const;
	PipeIn copy() const;
	void push(std::shared_ptr<const data::Message> value);
private:
	core::ID address;
	std::shared_ptr<Pipe> pipe;
	size_t id;

	void connect();
};




}
}

