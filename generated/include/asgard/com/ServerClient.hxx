#pragma once

#include <asgard/mod/Client.h>


namespace asgard{
namespace com{


class ServerClient : public mod::Client{
public:
	ServerClient(const core::ID &destination);
	ServerClient(const std::string &destination);
	ServerClient(pipe::PipeIn &&destination);
};


}
}
