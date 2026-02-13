#pragma once


#include <asgard/data/Data.hxx>
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

	DataPacket();
	bool operator==(const DataPacket &other) const;
	void read_member(type::TypeReader &reader, const std::string &name, const code::Typecode &type) override;
	void write_member(type::TypeWriter &writer, const std::string &name) const override;
};


}
}
