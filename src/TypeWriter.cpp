#include <asgard/io/TypeWriter.h>

#include <stdexcept>
#include <cstring>


namespace asgard{
namespace io{


TypeWriter::TypeWriter(std::shared_ptr<OutputSource> source):
	BufferedOutput(source)
{
}


void TypeWriter::write_typecode(const typecode_t &type){
	length_t sub_types = 0;
	bool write_size = false;
	switch(type.code){
	case typecode_t::TYPE_LIST:
	case typecode_t::TYPE_OPTIONAL:
	case typecode_t::TYPE_POINTER:
		sub_types = 1;
		break;
	case typecode_t::TYPE_MAP:
	case typecode_t::TYPE_PAIR:
		sub_types = 2;
		break;
	case typecode_t::TYPE_TUPLE:
		sub_types = type.sub_types.size();
		write_size = true;
		break;
	default:
		sub_types = 0;
		break;
	}
	sub_types = std::min<length_t>(sub_types, type.sub_types.size());

	write_le(type.code);
	if(type.code == typecode_t::TYPE_VALUE || type.code == typecode_t::TYPE_ENUM){
		write_value(type.name);
	}
	if(write_size){
		write_le(sub_types);
	}
	for(size_t i=0; i<sub_types; i++){
		write_typecode(type.sub_types.at(i));
	}
}


void TypeWriter::write_value(const std::nullptr_t &){
}


void TypeWriter::write_value(const bool &value){
	const uint8_t v = value ? 1 : 0;
	write_le(v);
}


void TypeWriter::write_value(const uint8_t &value){
	write_le(value);
}


void TypeWriter::write_value(const int8_t &value){
	write_le(value);
}


void TypeWriter::write_value(const uint16_t &value){
	write_le(value);
}


void TypeWriter::write_value(const int16_t &value){
	write_le(value);
}


void TypeWriter::write_value(const uint32_t &value){
	write_le(value);
}


void TypeWriter::write_value(const int32_t &value){
	write_le(value);
}


void TypeWriter::write_value(const uint64_t &value){
	write_le(value);
}


void TypeWriter::write_value(const int64_t &value){
	write_le(value);
}


void TypeWriter::write_value(const float &value){
	write_le(value);
}


void TypeWriter::write_value(const double &value){
	write_le(value);
}


void TypeWriter::write_value(const std::string &value){
	const length_t length = value.size();
	write_le(length);
	for(const auto &chr : value){
		write(static_cast<uint8_t>(chr));
	}
}


void TypeWriter::write_value(const char *value){
	const std::string value_ = value;
	write_value(value_);
}


void TypeWriter::write_value(const time::duration &value){
	const int64_t ticks = std::chrono::duration_cast<time::resolution>(value).count();
	write_le(ticks);
}


void TypeWriter::write_value(const time::time &value){
	const auto since_epoch = value.time_since_epoch();
	const int64_t ticks = std::chrono::duration_cast<time::resolution>(since_epoch).count();
	write_le(ticks);
}


void TypeWriter::write_value(const time::wall_time &value){
	const auto since_epoch = value.time_since_epoch();
	const int64_t ticks = std::chrono::duration_cast<time::resolution>(since_epoch).count();
	write_le(ticks);
}


void TypeWriter::write_value(const data::Value &/*value*/){
	throw std::logic_error("Not implemented");
}


void TypeWriter::write_value(const data::Enum &value){
	write_value(value.to_string());
}


}
}
