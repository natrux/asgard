#pragma once

#include <asgard/data/Value.h>


namespace asgard{
namespace data{


class Message : public Value{
	using Super = Value;
public:
	static code::Signature static_signature();
	static std::shared_ptr<Message> create();

	Message();
	void read_member(io::TypeReader &reader, const std::string &name, const code::Typecode &type) override;
	void write_member(io::TypeWriter &writer, const std::string &name) const override;
};


}
}