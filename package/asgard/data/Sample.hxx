#pragma once

#include <asgard/data/Message.hxx>
#include <asgard/time/time.h>
#include <asgard/data/Data.hxx>


namespace asgard{
namespace data{


class Sample : public Message{
	using Super = Message;
public:
	Sample();
	void read_member(type::TypeReader &reader, const std::string &name, const code::Typecode &type) override;
	void write_member(type::TypeWriter &writer, const std::string &name) const override;

	time::time time;
	bool retained = false;
	std::shared_ptr<const Data> data;

	static code::Signature static_signature();
	static std::shared_ptr<Sample> create();
};


}
}
