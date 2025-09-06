#include <asgard/io/TypeReader.h>

#include <stdexcept>


namespace asgard{
namespace io{


TypeReader::TypeReader(std::shared_ptr<InputSource> source):
	BufferedInput(source)
{
}


time::time TypeReader::get_remote_epoch() const{
	return remote_epoch;
}


void TypeReader::set_remote_epoch(const time::time &time){
	remote_epoch = time;
}


void TypeReader::set_remote_since_epoch(const time::duration &since){
	set_remote_epoch(time::now() - since);
}


code::Typecode TypeReader::read_typecode(){
	code::Typecode result;
	result.code = read_le<code::typecode_e>();
	if(result.code == code::Typecode::TYPE_VALUE || result.code == code::Typecode::TYPE_ENUM){
		read_string(result.name);
	}
	switch(result.code){
	case code::Typecode::TYPE_LIST:
	case code::Typecode::TYPE_OPTIONAL:
	case code::Typecode::TYPE_POINTER:
		result.sub_types.push_back(read_typecode());
		break;
	case code::Typecode::TYPE_MAP:
	case code::Typecode::TYPE_PAIR:
		result.sub_types.push_back(read_typecode());
		result.sub_types.push_back(read_typecode());
		break;
	case code::Typecode::TYPE_TUPLE:{
		const auto size = read_le<code::length_t>();
		for(size_t i=0; i<size; i++){
			result.sub_types.push_back(read_typecode());
		}
		break;
	}
	default:
		break;
	}
	return result;
}


void TypeReader::read_type(bool &value, const code::Typecode &type){
	uint8_t byte = 0;
	if(type.code == code::Typecode::TYPE_BOOL){
		read(byte);
	}else{
		read_number(byte, type.code);
	}
	value = (byte != 0);
}


void TypeReader::read_type(uint8_t &value, const code::Typecode &type){
	read_number(value, type.code);
}


void TypeReader::read_type(int8_t &value, const code::Typecode &type){
	read_number(value, type.code);
}


void TypeReader::read_type(uint16_t &value, const code::Typecode &type){
	read_number(value, type.code);
}


void TypeReader::read_type(int16_t &value, const code::Typecode &type){
	read_number(value, type.code);
}


void TypeReader::read_type(uint32_t &value, const code::Typecode &type){
	read_number(value, type.code);
}


void TypeReader::read_type(int32_t &value, const code::Typecode &type){
	read_number(value, type.code);
}


void TypeReader::read_type(uint64_t &value, const code::Typecode &type){
	read_number(value, type.code);
}


void TypeReader::read_type(int64_t &value, const code::Typecode &type){
	read_number(value, type.code);
}


void TypeReader::read_type(float &value, const code::Typecode &type){
	read_number(value, type.code);
}


void TypeReader::read_type(double &value, const code::Typecode &type){
	read_number(value, type.code);
}


void TypeReader::read_type(std::string &value, const code::Typecode &type){
	if(type.code == code::Typecode::TYPE_STRING){
		read_string(value);
	//}else if(code == code::Typecode::TYPE_ENUM){
	}else{
		skip(type);
	}
}


void TypeReader::read_type(time::time &value, const code::Typecode &type){
	if(type.code == code::Typecode::TYPE_DURATION){
		int64_t ticks = 0;
		read_number(ticks, type.code);
		value = remote_epoch + time::resolution(ticks);
	}else{
		skip(type);
	}
}


void TypeReader::read_type(time::wall_time &value, const code::Typecode &type){
	if(type.code == code::Typecode::TYPE_DURATION){
		int64_t ticks = 0;
		read_number(ticks, type.code);
		value = time::epoch_wall() + time::resolution(ticks);
	}else{
		skip(type);
	}
}


void TypeReader::read_type(time::duration &value, const code::Typecode &type){
	if(type.code == code::Typecode::TYPE_DURATION){
		int64_t ticks = 0;
		read_number(ticks, type.code);
		value = time::resolution(ticks);
	}else{
		skip(type);
	}
}


void TypeReader::read_type(data::Value &/*value*/, const code::Typecode &/*type*/){
	throw std::logic_error("Not implemented");
}


void TypeReader::read_type(data::Enum &value, const code::Typecode &type){
	if(type.code == code::Typecode::TYPE_ENUM || type.code == code::Typecode::TYPE_STRING){
		std::string field;
		read_string(field);
		value.from_string(field);
	}else{
		skip(type);
	}
}


void TypeReader::skip(){
	const auto type = read_typecode();
	skip(type);
}


void TypeReader::skip(const code::Typecode &type){
	switch(type.code){
	case code::Typecode::TYPE_NULL: break;
	case code::Typecode::TYPE_BOOL:
	case code::Typecode::TYPE_U8:
	case code::Typecode::TYPE_I8: read(1); break;
	case code::Typecode::TYPE_U16:
	case code::Typecode::TYPE_I16: read(2); break;
	case code::Typecode::TYPE_U32:
	case code::Typecode::TYPE_I32: read(4); break;
	case code::Typecode::TYPE_U64:
	case code::Typecode::TYPE_I64: read(8); break;
	case code::Typecode::TYPE_F32: read(4); break;
	case code::Typecode::TYPE_F64: read(8); break;
	case code::Typecode::TYPE_ENUM:
	case code::Typecode::TYPE_STRING:{
		const auto size = read_le<code::length_t>();
		read(size);
		break;
	}
	case code::Typecode::TYPE_LIST:{
		const auto size = read_le<code::length_t>();
		const auto &sub_type = type.sub_types.at(0);
		for(size_t i=0; i<size; i++){
			skip(sub_type);
		}
		break;
	}
	case code::Typecode::TYPE_MAP:{
		const auto size = read_le<code::length_t>();
		const auto &key_type = type.sub_types.at(0);
		const auto &value_type = type.sub_types.at(1);
		for(size_t i=0; i<size; i++){
			skip(key_type);
			skip(value_type);
		}
		break;
	}
	case code::Typecode::TYPE_PAIR:
		skip(type.sub_types.at(0));
		skip(type.sub_types.at(1));
		break;
	case code::Typecode::TYPE_TUPLE:{
		const auto size = type.sub_types.size();
		for(size_t i=0; i<size; i++){
			skip(type.sub_types.at(i));
		}
		break;
	}
	case code::Typecode::TYPE_DURATION: read(8); break;
	case code::Typecode::TYPE_OPTIONAL:
	case code::Typecode::TYPE_POINTER:{
		const bool flag = read_le<uint8_t>();
		if(flag){
			skip(type.sub_types.at(0));
		}
		break;
	}
	case code::Typecode::TYPE_VALUE:
		// TODO
		break;
	default:
		// suppress warning
		throw std::runtime_error("attempt to skip unknown type");
	}
}


void TypeReader::read_string(std::string &value){
	const auto size = read_le<code::length_t>();
	const auto chrs = read(size);
	value.append(chrs.begin(), chrs.end());
}


}
}
