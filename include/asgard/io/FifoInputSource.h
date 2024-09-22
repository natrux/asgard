#pragma once

#include <asgard/io/InputSource.h>
#include <asgard/util/Fifo.h>

#include <memory>


namespace asgard{
namespace io{


class FifoInputSource : public InputSource{
public:
	FifoInputSource(std::shared_ptr<util::Fifo> source);
	size_t read(void *data, size_t size) override;

private:
	std::shared_ptr<util::Fifo> source;
};


}
}
