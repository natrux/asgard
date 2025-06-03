#include <asgard/io/TypeReader.h>


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


void TypeReader::set_remote_epoch(const time::duration &since_remote_epoch){
	set_remote_epoch(time::now() - since_remote_epoch);
}


typecode_t TypeReader::read_typecode(){
	typecode_t result;
	result.code = read_le<typecode_e>();
	switch(result.code){
	case typecode_t::TYPE_LIST:
	case typecode_t::TYPE_OPTIONAL:
	case typecode_t::TYPE_POINTER:
		result.sub_types.push_back(read_typecode());
		break;
	case typecode_t::TYPE_MAP:
	case typecode_t::TYPE_PAIR:
		result.sub_types.push_back(read_typecode());
		result.sub_types.push_back(read_typecode());
		break;
	case typecode_t::TYPE_TUPLE:{
		const auto size = read_le<length_t>();
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


void TypeReader::read_type(bool &value, const typecode_t &type){
	uint8_t byte = 0;
	if(type.code == typecode_t::TYPE_BOOL){
		read(byte);
	}else{
		read_number(byte, type.code);
	}
	value = (byte != 0);
}


void TypeReader::read_type(uint8_t &value, const typecode_t &type){
	read_number(value, type.code);
}


void TypeReader::read_type(int8_t &value, const typecode_t &type){
	read_number(value, type.code);
}


void TypeReader::read_type(uint16_t &value, const typecode_t &type){
	read_number(value, type.code);
}


void TypeReader::read_type(int16_t &value, const typecode_t &type){
	read_number(value, type.code);
}


void TypeReader::read_type(uint32_t &value, const typecode_t &type){
	read_number(value, type.code);
}


void TypeReader::read_type(int32_t &value, const typecode_t &type){
	read_number(value, type.code);
}


void TypeReader::read_type(uint64_t &value, const typecode_t &type){
	read_number(value, type.code);
}


void TypeReader::read_type(int64_t &value, const typecode_t &type){
	read_number(value, type.code);
}


void TypeReader::read_type(float &value, const typecode_t &type){
	read_number(value, type.code);
}


void TypeReader::read_type(double &value, const typecode_t &type){
	read_number(value, type.code);
}


void TypeReader::read_type(std::string &value, const typecode_t &type){
	if(type.code == typecode_t::TYPE_STRING){
		const auto size = read_le<length_t>();
		const auto chrs = read(size);
		value.append(chrs.begin(), chrs.end());
	//}else if(code == typecode_t::TYPE_ENUM){
	}else{
		skip(type);
	}
}


void TypeReader::read_type(time::time &value, const typecode_t &type){
	if(type.code == typecode_t::TYPE_DURATION){
		int64_t ticks = 0;
		read_number(ticks, type.code);
		value = remote_epoch + time::resolution(ticks);
	}else{
		skip(type);
	}
}


void TypeReader::read_type(time::wall_time &value, const typecode_t &type){
	if(type.code == typecode_t::TYPE_DURATION){
		int64_t ticks = 0;
		read_number(ticks, type.code);
		value = time::wall_time() + time::resolution(ticks);
	}else{
		skip(type);
	}
}


void TypeReader::read_type(time::duration &value, const typecode_t &type){
	if(type.code == typecode_t::TYPE_DURATION){
		int64_t ticks = 0;
		read_number(ticks, type.code);
		value = time::resolution(ticks);
	}else{
		skip(type);
	}
}


void TypeReader::skip(){
	const auto type = read_typecode();
	skip(type);
}


void TypeReader::skip(const typecode_t &type){
	switch(type.code){
	case typecode_t::TYPE_NULL: break;
	case typecode_t::TYPE_BOOL:
	case typecode_t::TYPE_U8:
	case typecode_t::TYPE_I8: read(1); break;
	case typecode_t::TYPE_U16:
	case typecode_t::TYPE_I16: read(2); break;
	case typecode_t::TYPE_U32:
	case typecode_t::TYPE_I32: read(4); break;
	case typecode_t::TYPE_U64:
	case typecode_t::TYPE_I64: read(8); break;
	case typecode_t::TYPE_F32: read(4); break;
	case typecode_t::TYPE_F64: read(8); break;
	case typecode_t::TYPE_STRING:{
		const auto size = read_le<length_t>();
		read(size);
		break;
	}
	case typecode_t::TYPE_LIST:{
		const auto size = read_le<length_t>();
		for(size_t i=0; i<size; i++){
			skip(type.sub_types.at(0));
		}
		break;
	}
	case typecode_t::TYPE_MAP:{
		const auto size = read_le<length_t>();
		for(size_t i=0; i<size; i++){
			skip(type.sub_types.at(0));
			skip(type.sub_types.at(1));
		}
		break;
	}
	case typecode_t::TYPE_PAIR:
		skip(type.sub_types.at(0));
		skip(type.sub_types.at(1));
		break;
	case typecode_t::TYPE_TUPLE:{
		const auto size = type.sub_types.size();
		for(size_t i=0; i<size; i++){
			skip(type.sub_types.at(i));
		}
		break;
	}
	case typecode_t::TYPE_DURATION: read(8); break;
	}
}


}
}
