#include <asgard/fs/File.h>

#include <stdexcept>


namespace asgard{
namespace fs{


File::File(const std::string &path_):
	path(path_)
{
}


std::shared_ptr<io::FileInputSource> File::input_source() const{
	return std::make_shared<io::FileInputSource>(path);
}


std::shared_ptr<io::FileOutputSource> File::output_source(bool append) const{
	return std::make_shared<io::FileOutputSource>(path, append);
}


}
}
