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


void Bin::set_data(std::vector<uint8_t> &&data_){
	data = std::move(data_);
}


code::Typecode Bin::get_type() const{
	return type;
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


bool Bin::is_number() const{
	return is_integer() || is_floating_point();
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


bool Bin::is_pointer() const{
	return type.code == code::Typecode::TYPE_POINTER;
}


bool Bin::is_value() const{
	return type.code == code::Typecode::TYPE_VALUE;
}


bool Bin::is_enum() const{
	return type.code == code::Typecode::TYPE_ENUM;
}


void Bin::update(){
	auto input = std::make_shared<io::VectorInputSource>(data);
	type::TypeReader reader(input);
	type = reader.read_typecode();
	if(type.code == code::Typecode::TYPE_VALUE){
		signature = reader.read_signature();
	}
}


}
}
