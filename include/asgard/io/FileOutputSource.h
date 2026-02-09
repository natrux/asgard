#pragma once

#include <asgard/io/OutputSource.h>

#include <string>
#include <cstdio>


namespace asgard{
namespace io{


class FileOutputSource : public OutputSource{
public:
	FileOutputSource(const std::string &path, bool append=false);
	FileOutputSource(const FileOutputSource &other) = delete;
	~FileOutputSource();
	FileOutputSource operator=(const FileOutputSource &other) = delete;

	size_t write(const void *data, size_t length) override;

private:
	std::FILE *stream;
};


}
}

