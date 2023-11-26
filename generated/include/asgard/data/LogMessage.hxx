#pragma once

#include <asgard/data/Data.hxx>
#include <asgard/time/time.h>
#include <asgard/data/log_level_e.hxx>

#include <string>


namespace asgard{
namespace data{


class LogMessage : public Data{
public:
	time::time time;
	log_level_e level;
	std::string unit;
	std::string message;

	std::string format() const;
};


}
}

