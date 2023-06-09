#pragma once

#include <asgard/data/Request.hxx>
#include <asgard/run/terminal_event_e.hxx>


namespace asgard{
namespace run{


class Terminal_read_event : public data::Request{
public:
	Terminal_read_event(const terminal_event_e &event_);

	terminal_event_e event;
};


}
}
