#pragma once

#include <asgard/mod/Client.h>


namespace asgard{
namespace com{


class NetworkStreamClient : public mod::Client{
public:
	NetworkStreamClient(const core::ID &destination);
	NetworkStreamClient(const std::string &destination);
	NetworkStreamClient(pipe::PipeIn &&destination);
};


}
}
