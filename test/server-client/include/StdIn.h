#pragma once


#include <asgard/mod/Module.h>
#include <asgard/topic/TopicPtr.h>


class StdIn : public asgard::mod::Module{
public:
	StdIn(const std::string &name_);

protected:
	void main() override;

private:
	typedef Module Super;

	asgard::topic::TopicPtr output_lines = "input.terminal";
	void read_loop();
};


