#pragma once

#include <asgard/data/Value.h>
#include <asgard/data/log_level_e.hxx>

#include <string>


namespace asgard{
namespace data{


class LogMessage : public Value{
public:
	log_level_e level;
	std::string unit;
	std::string message;

	std::string format() const;
};


}
}

