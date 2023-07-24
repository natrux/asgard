#pragma once

#include <asgard/mod/Module.h>
#include <cstdint>


namespace asgard{
namespace com{


class GatewayModule : public mod::Module{
public:
	using Super = mod::Module;

	GatewayModule(const std::string &name_);

protected:
	uint32_t error_pause_time_ms = 1000;
};


}
}
