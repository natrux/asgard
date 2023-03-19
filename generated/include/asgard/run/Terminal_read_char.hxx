#pragma once

#include <asgard/data/Request.hxx>


namespace asgard{
namespace run{


class Terminal_read_char : public data::Request{
public:
	Terminal_read_char(const char &character_);

	char character;
};


}
}

