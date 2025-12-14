#pragma once

#include <asgard/code/Signature.h>

#include <memory>


namespace asgard{

namespace io{
class TypeReader;
class TypeWriter;
}

namespace data{


class Value{
public:
	static code::Signature static_signature();
	static std::shared_ptr<Value> create();

	virtual ~Value() = default;
	virtual code::Signature signature() const;

	void read_member(io::TypeReader &reader, const std::string &name, const code::Typecode &code);
	void write_member(io::TypeWriter &writer, const std::string &name) const;

protected:
	virtual bool try_read_member(io::TypeReader &reader, const std::string &name, const code::Typecode &code);
	virtual bool try_write_member(io::TypeWriter &writer, const std::string &name) const;
};



}
}
