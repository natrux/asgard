#pragma once

#include <asgard/code/Signature.h>
#include <asgard/data/Bin.h>

#include <memory>


namespace asgard{
namespace io{
class TypeReader;
class TypeWriter;
}
}


namespace asgard{
namespace data{


class Value{
public:
	static code::Signature static_signature();
	static std::shared_ptr<Value> create();

	Value();
	virtual ~Value() = default;
	code::Signature signature() const;
	void set_signature(const code::Signature &signature);

	virtual void read_member(io::TypeReader &reader, const std::string &name, const code::Typecode &type);
	virtual void write_member(io::TypeWriter &writer, const std::string &name) const;

private:
	code::Signature dynamic_signature;
	std::map<std::string, Bin> unknown_members;
};



}
}
