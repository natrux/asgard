#pragma once

#include <asgard/code/Signature.h>


namespace asgard{

namespace io{
class TypeReader;
class TypeWriter;
}

namespace data{


class Value{
public:
	static code::Signature static_signature();

	virtual ~Value() = default;
	virtual code::Signature signature() const;

	void read_member(io::TypeReader &reader, const std::string &name);
	void write_member(io::TypeWriter &writer, const std::string &name) const;

protected:
	virtual bool try_read_member(io::TypeReader &reader, const std::string &name);
	virtual bool try_write_member(io::TypeWriter &writer, const std::string &name) const;
};



}
}
