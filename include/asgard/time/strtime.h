#pragma once

#include <asgard/time/time.h>

#include <string>


namespace asgard{
namespace time{


std::string strtime(const duration &period, size_t terms=2);
std::string strtime(const time &time, size_t terms=2);
std::string strtime(const wall_time &time, size_t terms=2);


}
}
