#pragma once


#include <asgard/data/Value.h>


namespace asgard{
namespace data{

class Data : public Value{
	using Super = Value;
public:
	static code::Signature static_signature();
	code::Signature signature() const override;

protected:
	bool try_read_member(io::TypeReader &reader, const std::string &name) override;
	bool try_write_member(io::TypeWriter &writer, const std::string &name) const override;
};


}
}
