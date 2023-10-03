#pragma once

#include <asgard/core/ID.h>
#include <asgard/data/Message.hxx>

#include <memory>

namespace asgard{
namespace pipe{


class Pipe;

class PipeIn{
public:
	PipeIn() = default;
	PipeIn(std::shared_ptr<Pipe> pipe);
	PipeIn(const PipeIn &other) = delete;
	PipeIn(PipeIn &&other) = default;
	PipeIn &operator=(const PipeIn &other) = delete;
	PipeIn &operator=(PipeIn &&other);

	bool is_connected() const;
	PipeIn copy() const;
	void push(std::shared_ptr<const data::Message> value) const;

	struct Compare{
		std::less<const std::shared_ptr<Pipe> &> is_less;
		bool operator()(const PipeIn &a, const PipeIn &b) const;
	};
private:
	std::shared_ptr<Pipe> pipe;
	size_t id;
};




}
}

