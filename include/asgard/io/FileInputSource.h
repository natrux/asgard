#pragma once

#include <asgard/io/InputSource.h>

#include <string>
#include <cstdio>


namespace asgard{
namespace io{


class FileInputSource : public InputSource{
public:
	FileInputSource(const std::string &path);
	FileInputSource(const FileInputSource &other) = delete;
	~FileInputSource();
	FileInputSource &operator=(const FileInputSource &other) = delete;

	size_t read(void *data, size_t length) override;

private:
	std::FILE *stream;
};


}
}

