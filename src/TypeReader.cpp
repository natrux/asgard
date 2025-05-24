#include <asgard/io/TypeReader.h>


namespace asgard{
namespace io{


TypeReader::TypeReader(std::shared_ptr<InputSource> source):
	BufferedInput(source)
{
}


time::duration TypeReader::get_delta_time() const{
	return delta_time;
}


void TypeReader::set_delta_time(const time::duration &delta){
	delta_time = delta;
}


void TypeReader::read_type(bool &value, typecode_e code){
	uint8_t byte = 0;
	if(code == typecode_t::TYPE_BOOL){
		read(byte);
	}else{
		read_number(byte, code);
	}
	value = (byte != 0);
}


void TypeReader::read_type(uint8_t &value, typecode_e code){
	read_number(value, code);
}


void TypeReader::read_type(int8_t &value, typecode_e code){
	read_number(value, code);
}


void TypeReader::read_type(uint16_t &value, typecode_e code){
	read_number(value, code);
}


void TypeReader::read_type(int16_t &value, typecode_e code){
	read_number(value, code);
}


void TypeReader::read_type(uint32_t &value, typecode_e code){
	read_number(value, code);
}


void TypeReader::read_type(int32_t &value, typecode_e code){
	read_number(value, code);
}


void TypeReader::read_type(uint64_t &value, typecode_e code){
	read_number(value, code);
}


void TypeReader::read_type(int64_t &value, typecode_e code){
	read_number(value, code);
}


void TypeReader::read_type(float &value, typecode_e code){
	read_number(value, code);
}


void TypeReader::read_type(double &value, typecode_e code){
	read_number(value, code);
}


void TypeReader::read_type(std::string &value, typecode_e code){
	if(code == typecode_t::TYPE_STRING){
		const uint64_t size = read_le<uint64_t>();
		const auto chrs = read(size);
		value.append(chrs.begin(), chrs.end());
	//}else if(code == typecode_t::TYPE_ENUM){
	}else{
		skip(code);
	}
}


void TypeReader::read_type(time::time &value, typecode_e code){
	if(code == typecode_t::TYPE_DURATION){
		int64_t ticks = 0;
		read_number(ticks, code);
		value = time::time() + time::resolution(ticks) + delta_time;
	}else{
		skip(code);
	}
}


void TypeReader::read_type(time::wall_time &value, typecode_e code){
	if(code == typecode_t::TYPE_DURATION){
		int64_t ticks = 0;
		read_number(ticks, code);
		value = time::wall_time() + time::resolution(ticks);
	}else{
		skip(code);
	}
}


void TypeReader::read_type(time::duration &value, typecode_e code){
	if(code == typecode_t::TYPE_DURATION){
		int64_t ticks = 0;
		read_number(ticks, code);
		value = time::resolution(ticks);
	}else{
		skip(code);
	}
}


void TypeReader::skip(){
	const auto code = read_le<typecode_e>();
	skip(code);
}


void TypeReader::skip(typecode_e code){
	switch(code){
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
		const auto size = read_le<uint64_t>();
		read(size);
		break;
	}
	case typecode_t::TYPE_LIST:{
		const auto size = read_le<uint64_t>();
		for(size_t i=0; i<size; i++){
			skip();
		}
		break;
	}
	case typecode_t::TYPE_MAP:{
		const auto size = read_le<uint64_t>();
		for(size_t i=0; i<size; i++){
			skip();
			skip();
		}
		break;
	}
	case typecode_t::TYPE_PAIR: skip(); skip(); break;
	case typecode_t::TYPE_TUPLE:{
		const auto size = read_le<uint64_t>();
		for(size_t i=0; i<size; i++){
			skip();
		}
		break;
	}
	case typecode_t::TYPE_DURATION: read(8); break;
	}
}


}
}
