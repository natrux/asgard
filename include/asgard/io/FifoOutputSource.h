#pragma once

#include <asgard/io/OutputSource.h>
#include <asgard/util/Fifo.h>


namespace asgard{
namespace io{


class FifoOutputSource : public OutputSource{
public:
	FifoOutputSource(std::shared_ptr<util::Fifo> source);

	size_t write(const void *data, size_t length) override;

private:
	std::shared_ptr<util::Fifo> source;
};


}
}
