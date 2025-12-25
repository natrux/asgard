#include <asgard/data/Bin.h>


namespace asgard{
namespace data{


Bin::Bin():
	Bin(nullptr)
{}


std::vector<uint8_t> Bin::get_data() const{
	return data;
}


void Bin::set_data(const std::vector<uint8_t> &data_){
	data = data_;
}


bool Bin::is_empty() const{
	return type.code == code::Typecode::TYPE_NULL;
}


bool Bin::is_bool() const{
	return type.code == code::Typecode::TYPE_BOOL;
}


bool Bin::is_integer() const{
	return
		type.code == code::Typecode::TYPE_U8 ||
		type.code == code::Typecode::TYPE_I8 ||
		type.code == code::Typecode::TYPE_U16 ||
		type.code == code::Typecode::TYPE_I16 ||
		type.code == code::Typecode::TYPE_U32 ||
		type.code == code::Typecode::TYPE_I32 ||
		type.code == code::Typecode::TYPE_U64 ||
		type.code == code::Typecode::TYPE_I64 ||
	false;
}


bool Bin::is_unsigned_integer() const{
	return
		type.code == code::Typecode::TYPE_U8 ||
		type.code == code::Typecode::TYPE_U16 ||
		type.code == code::Typecode::TYPE_U32 ||
		type.code == code::Typecode::TYPE_U64 ||
	false;
}


bool Bin::is_floating_point() const{
	return
		type.code == code::Typecode::TYPE_F32 ||
		type.code == code::Typecode::TYPE_F64 ||
	false;
}


bool Bin::is_string() const{
	return type.code == code::Typecode::TYPE_STRING;
}


bool Bin::is_list() const{
	return
		type.code == code::Typecode::TYPE_LIST ||
		type.code == code::Typecode::TYPE_PAIR ||
		type.code == code::Typecode::TYPE_TUPLE ||
	false;
}


bool Bin::is_map() const{
	return type.code == code::Typecode::TYPE_MAP;
}


void Bin::update(){
	auto input = std::make_shared<io::VectorInputSource>(data);
	io::TypeReader reader(input);
	type = reader.read_typecode();
	if(type.code == code::Typecode::TYPE_VALUE){
		signature = reader.read_signature();
	}
}


}
}
