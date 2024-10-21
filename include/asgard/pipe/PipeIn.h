#pragma once

#include <asgard/core/ID.h>
#include <asgard/data/Message.hxx>

#include <memory>
#include <optional>

namespace asgard{
namespace pipe{


class Pipe;

class PipeIn{
public:
	PipeIn() = default;
	PipeIn(const core::ID &address);
	PipeIn(std::shared_ptr<Pipe> pipe);
	PipeIn(const core::ID &address, std::shared_ptr<Pipe> pipe);
	PipeIn(const PipeIn &other) = delete;
	PipeIn(PipeIn &&other);
	PipeIn &operator=(const PipeIn &other) = delete;
	PipeIn &operator=(PipeIn &&other);

	bool operator==(const PipeIn &other) const;

	PipeIn copy() const;
	void push(std::shared_ptr<const data::Message> value);
private:
	std::optional<core::ID> address;
	std::weak_ptr<Pipe> pipe;
	size_t id;

	std::shared_ptr<Pipe> connect();
};




}
}

