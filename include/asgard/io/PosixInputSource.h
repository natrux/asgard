#pragma once

#include <asgard/io/InputSource.h>


namespace asgard{
namespace io{


class PosixInputSource : public InputSource{
public:
	PosixInputSource(int fd);
	size_t read(void *data, size_t length) override;

private:
	int m_fd;
};


}
}

