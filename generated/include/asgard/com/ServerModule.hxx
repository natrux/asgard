#pragma once

#include <asgard/mod/Module.h>


namespace asgard{
namespace com{


class ServerModule : public mod::Module{
public:
	using Super = mod::Module;

	ServerModule(const std::string &name_);
};


}
}
