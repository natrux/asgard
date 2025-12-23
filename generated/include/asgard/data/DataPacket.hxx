#pragma once


#include <asgard/data/Data.hxx>
#include <asgard/code/get_typecode.h>
#include <asgard/time/time.h>

#include <vector>
#include <cstdint>


namespace asgard{
namespace data{

class DataPacket : public Data{
	using Super = Data;
public:
	time::time time;
	std::vector<uint8_t> payload;

	static code::Signature static_signature();
	static std::shared_ptr<DataPacket> create();
	code::Signature signature() const override;
	bool operator==(const DataPacket &other) const;
	void read_member(io::TypeReader &reader, const std::string &name, const code::Typecode &code) override;
	void write_member(io::TypeWriter &writer, const std::string &name) const override;
};


}
}


namespace asgard{
namespace code{


template<>
struct get_typecode_t<asgard::data::DataPacket>{
	static Typecode value(){
		static Typecode instance(Typecode::TYPE_VALUE);
		return instance;
	}
};


}
}
