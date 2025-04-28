#include <asgard/io/TypeWriter.h>

#include <cstring>


namespace asgard{
namespace io{


TypeWriter::TypeWriter(std::shared_ptr<OutputSource> source):
	BufferedOutput(source)
{
}


void TypeWriter::write_type(const std::nullptr_t &){
	write_le(TYPE_NULL);
}


void TypeWriter::write_type(const bool &value){
	const uint8_t v = value ? 1 : 0;
	write_le(TYPE_BOOL);
	write_le(v);
}


void TypeWriter::write_type(const uint8_t &value){
	write_le(TYPE_U8);
	write_le(value);
}


void TypeWriter::write_type(const int8_t &value){
	write_le(TYPE_I8);
	write_le(value);
}


void TypeWriter::write_type(const uint16_t &value){
	write_le(TYPE_U16);
	write_le(value);
}


void TypeWriter::write_type(const int16_t &value){
	write_le(TYPE_I16);
	write_le(value);
}


void TypeWriter::write_type(const uint32_t &value){
	write_le(TYPE_U32);
	write_le(value);
}


void TypeWriter::write_type(const int32_t &value){
	write_le(TYPE_I32);
	write_le(value);
}


void TypeWriter::write_type(const uint64_t &value){
	write_le(TYPE_U64);
	write_le(value);
}


void TypeWriter::write_type(const int64_t &value){
	write_le(TYPE_I64);
	write_le(value);
}


void TypeWriter::write_type(const float &value){
	uint32_t tmp = 0;
	std::memcpy(&tmp, &value, 4);
	write_le(TYPE_F32);
	write_le(tmp);
}


void TypeWriter::write_type(const double &value){
	uint64_t tmp = 0;
	std::memcpy(&tmp, &value, 8);
	write_le(TYPE_F64);
	write_le(tmp);
}


void TypeWriter::write_type(const std::string &value){
	const uint64_t length = value.size();
	write_le(TYPE_STRING);
	write_le(length);
	for(const auto &chr : value){
		write(static_cast<uint8_t>(chr));
	}
}


void TypeWriter::write_type(const time::duration &value){
	const int64_t ticks = std::chrono::duration_cast<time::resolution>(value).count();
	write_le(TYPE_DURATION);
	write_le(ticks);
}


void TypeWriter::write_type(const time::time &value){
	const auto since_epoch = value.time_since_epoch();
	const int64_t ticks = std::chrono::duration_cast<time::resolution>(since_epoch).count();
	write_le(TYPE_DURATION);
	write_le(ticks);
}


void TypeWriter::write_type(const time::wall_time &value){
	const auto since_epoch = value.time_since_epoch();
	const int64_t ticks = std::chrono::duration_cast<time::resolution>(since_epoch).count();
	write_le(TYPE_DURATION);
	write_le(ticks);
}


}
}
