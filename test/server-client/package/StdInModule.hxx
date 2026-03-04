#pragma once

#include <asgard/mod/Module.h>
#include <asgard/topic/Topic.h>


class StdInModule : public asgard::mod::Module{
public:
	using Super = asgard::mod::Module;

	StdInModule(const std::string &name_);

protected:
	asgard::topic::TopicPtr output_lines = "input.terminal";
};

