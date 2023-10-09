#pragma once

#include <asgard/data/Message.hxx>
#include <asgard/time/time.h>
#include <asgard/data/Data.hxx>

#include <memory>


namespace asgard{
namespace data{


class Sample : public Message{
public:
	time::time time;
	std::shared_ptr<const Data> data;
};


}
}
