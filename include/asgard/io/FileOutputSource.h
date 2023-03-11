#pragma once

#include <asgard/io/OutputSource.h>


namespace asgard{
namespace io{


class FileOutputSource : public OutputSource{
public:
	FileOutputSource(int fd);
	size_t write(const char *data, size_t length) override;

private:
	int m_fd;
};


}
}

