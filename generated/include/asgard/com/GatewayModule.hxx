#pragma once

#include <asgard/mod/Module.h>
#include <asgard/time/time.h>
#include <cstdint>


namespace asgard{
namespace com{


class GatewayModule : public mod::Module{
public:
	using Super = mod::Module;

	GatewayModule(const std::string &name_);

protected:
	time::duration error_pause_time = std::chrono::seconds(1);
};


}
}
