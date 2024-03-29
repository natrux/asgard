#pragma once

#include <asgard/mod/Module.h>
#include <asgard/core/ReturnMe.h>
#include <asgard/run/terminal_event_e.hxx>
#include <asgard/data/LogMessage.hxx>
#include <asgard/run/Terminal_read_char_return.hxx>
#include <asgard/run/Terminal_read_event_return.hxx>


namespace asgard{
namespace run{

class TerminalModule : public mod::Module{
public:
	using Super = mod::Module;

	TerminalModule(const std::string &name_);

protected:
	topic::TopicPtr input_log = "asgard.log";
	std::string prompt = "> ";

	virtual void read_char_sync(const int8_t &character);
	virtual void read_char_async(const int8_t &character, core::ReturnMe<Terminal_read_char_return> &&return_me);
	virtual void read_event_sync(const terminal_event_e &event);
	virtual void read_event_async(const terminal_event_e &event, core::ReturnMe<Terminal_read_event_return> &&return_me);

	using Super::process;
	void process(std::shared_ptr<const data::Request> request) override;
	void process(std::shared_ptr<const data::Sample> sample) override;
	virtual void process(std::shared_ptr<const data::Sample> sample, std::shared_ptr<const data::LogMessage> data);
	virtual void process(std::shared_ptr<const data::LogMessage> value);
};



}
}
