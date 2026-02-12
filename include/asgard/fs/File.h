#pragma once

#include <asgard/io/FileInputSource.h>
#include <asgard/io/FileOutputSource.h>

#include <memory>


namespace asgard{
namespace fs{


class File{
public:
	File(const std::string &path);

	std::shared_ptr<io::FileInputSource> input_source() const;
	std::shared_ptr<io::FileOutputSource> output_source(bool append=false) const;

private:
	std::string path;
};


}
}
