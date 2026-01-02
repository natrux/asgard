#pragma once

#include <asgard/mod/Module.h>


namespace asgard{
namespace com{


class ServerModule : public mod::Module{
public:
	using Super = mod::Module;

	ServerModule(const std::string &name_);

protected:
	time::duration error_pause_time = 1s;
};


}
}
