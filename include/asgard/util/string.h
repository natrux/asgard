#pragma once

#include <string>
#include <vector>


namespace asgard{
namespace util{


std::vector<std::string> string_split(const std::string &string, char separator, size_t pos=0, bool skip_empty=false);

std::string string_join(const std::vector<std::string> &list, const std::string &glue);


}
}

