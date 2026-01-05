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
	void read_member(io::TypeReader &reader, const std::string &name, const code::Typecode &type) override;
	void write_member(io::TypeWriter &writer, const std::string &name) const override;

	time::time time;
	bool retained = false;
	std::shared_ptr<const Data> data;

	static code::Signature static_signature();
	static std::shared_ptr<Sample> create();
};


}
}


namespace asgard{
namespace code{


template<>
struct get_typecode_t<asgard::data::Sample>{
	static Typecode value(){
		static Typecode instance(Typecode::TYPE_VALUE);
		return instance;
	}
};


}
}
