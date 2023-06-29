#pragma once

#include <asgard/data/Message.hxx>
#include <asgard/data/Data.hxx>

#include <memory>


namespace asgard{
namespace data{


class Sample : public Message{
public:
	std::shared_ptr<const Data> data;
};


}
}
