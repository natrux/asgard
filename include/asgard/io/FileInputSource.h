#pragma once

#include <asgard/io/InputSource.h>


namespace asgard{
namespace io{


class FileInputSource : public InputSource{
public:
	FileInputSource(int fd);
	size_t read(void *data, size_t length) override;

private:
	int m_fd;
};


}
}

