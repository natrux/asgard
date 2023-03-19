#pragma once

#include <asgard/mod/Module.h>


namespace asgard{
namespace com{


class GatewayModule : public mod::Module{
public:
	using Super = mod::Module;

	GatewayModule(const std::string &name_);

protected:
	unsigned int error_pause_time_ms = 1000;
};


}
}
