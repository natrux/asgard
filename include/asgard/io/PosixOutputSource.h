#pragma once

#include <asgard/io/OutputSource.h>


namespace asgard{
namespace io{


class PosixOutputSource : public OutputSource{
public:
	PosixOutputSource(int fd);
	size_t write(const void *data, size_t length) override;

private:
	int m_fd;
};


}
}

