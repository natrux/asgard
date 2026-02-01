#include <asgard/io/TypeReader.h>
#include <asgard/io/TypeWriter.h>
#include <asgard/core/TypeRegistry.h>
#include <asgard/data/Value.h>
#include <asgard/data/Bin.h>

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


code::Signature TypeReader::read_signature(){
	const core::ID id(read_le<uint64_t>());
	code::Signature signature;
	if(read_bool()){
		signature.name = read_string();

		const auto num_parents = read_le<code::length_t>();
		for(code::length_t i=0; i<num_parents; i++){
			signature.parents.push_back(read_string());
		}

		const auto num_members = read_le<code::length_t>();
		for(code::length_t i=0; i<num_members; i++){
			const auto key = read_string();
			signature.members[key] = read_typecode();
		}

		signature_map[id] = signature;
	}else{
		const auto find = signature_map.find(id);
		if(find != signature_map.end()){
			signature = find->second;
		}
	}
	return signature;
}


code::EnumMap TypeReader::read_enum_map(){
	const core::ID id(read_le<uint64_t>());
	code::EnumMap map;
	if(read_bool()){
		map.name = read_string();

		const auto num_values = read_le<code::length_t>();
		for(code::length_t i=0; i<num_values; i++){
			const auto value = read_le<code::enum_t>();
			const auto name = read_string();
			map.enum_map[value] = name;
			map.reverse_enum_map[name] = value;
		}

		enum_maps[id] = map;
	}else{
		const auto find = enum_maps.find(id);
		if(find != enum_maps.end()){
			map = find->second;
		}
	}
	return map;
}


void TypeReader::read_type(bool &value, const code::Typecode &type){
	uint8_t byte = 0;
	if(type.code == code::Typecode::TYPE_BOOL){
		read(byte);
	}else{
		read_number(byte, type);
	}
	value = (byte != 0);
}


void TypeReader::read_type(uint8_t &value, const code::Typecode &type){
	read_number(value, type);
}


void TypeReader::read_type(int8_t &value, const code::Typecode &type){
	read_number(value, type);
}


void TypeReader::read_type(uint16_t &value, const code::Typecode &type){
	read_number(value, type);
}


void TypeReader::read_type(int16_t &value, const code::Typecode &type){
	read_number(value, type);
}


void TypeReader::read_type(uint32_t &value, const code::Typecode &type){
	read_number(value, type);
}


void TypeReader::read_type(int32_t &value, const code::Typecode &type){
	read_number(value, type);
}


void TypeReader::read_type(uint64_t &value, const code::Typecode &type){
	read_number(value, type);
}


void TypeReader::read_type(int64_t &value, const code::Typecode &type){
	read_number(value, type);
}


void TypeReader::read_type(float &value, const code::Typecode &type){
	read_number(value, type);
}


void TypeReader::read_type(double &value, const code::Typecode &type){
	read_number(value, type);
}


void TypeReader::read_type(std::string &value, const code::Typecode &type){
	if(type.code == code::Typecode::TYPE_STRING){
		value = read_string();
	//}else if(code == code::Typecode::TYPE_ENUM){
	}else{
		skip(type);
	}
}


void TypeReader::read_type(time::time &value, const code::Typecode &type){
	if(type.code == code::Typecode::TYPE_DURATION){
		int64_t ticks = 0;
		read_number(ticks, type);
		value = remote_epoch + time::resolution(ticks);
	}else{
		skip(type);
	}
}


void TypeReader::read_type(time::wall_time &value, const code::Typecode &type){
	if(type.code == code::Typecode::TYPE_DURATION){
		int64_t ticks = 0;
		read_number(ticks, type);
		value = time::epoch_wall() + time::resolution(ticks);
	}else{
		skip(type);
	}
}


void TypeReader::read_type(time::duration &value, const code::Typecode &type){
	if(type.code == code::Typecode::TYPE_DURATION){
		int64_t ticks = 0;
		read_number(ticks, type);
		value = time::resolution(ticks);
	}else{
		skip(type);
	}
}


void TypeReader::read_type(data::Value &value, const code::Typecode &type){
	if(type.code == code::Typecode::TYPE_VALUE){
		const auto signature = read_signature();
		read_type(value, signature);
	}else if(type.code == code::Typecode::TYPE_POINTER){
		const bool flag = read_bool();
		if(flag){
			read_type(value, type.sub_types.at(0));
		}
	}else{
		skip(type);
	}
}


void TypeReader::read_type(data::Value &value, const code::Signature &signature){
	for(const auto &entry : signature.members){
		value.read_member(*this, entry.first, entry.second);
	}
}


void TypeReader::read_type(data::Enum &value, const code::Typecode &type){
	if(type.code == code::Typecode::TYPE_ENUM || type.code == code::Typecode::TYPE_STRING){
		const std::string field = read_string();
		value.from_string(field);
	}else{
		skip(type);
	}
}


void TypeReader::read_type(data::Bin &value, const code::Typecode &type){
	auto source = std::make_shared<io::VectorOutputSource>();
	{
		TypeWriter out(source);
		out.write_typecode(type);
		copy(out, type);
	}
	const auto data = source->get();
	value.set_data(data);
}


void TypeReader::skip(){
	const auto type = read_typecode();
	skip(type);
}


void TypeReader::skip(const code::Typecode &type){
	TypeWriter out(nullptr);
	copy(out, type);
}


void TypeReader::copy(TypeWriter &out){
	const auto type = read_typecode();
	out.write_typecode(type);
	copy(out, type);
}


void TypeReader::copy(TypeWriter &out, const code::Typecode &type){
	switch(type.code){
	case code::Typecode::TYPE_NULL: break;
	case code::Typecode::TYPE_BOOL:
	case code::Typecode::TYPE_U8:
	case code::Typecode::TYPE_I8: out.write(read(1)); break;
	case code::Typecode::TYPE_U16:
	case code::Typecode::TYPE_I16: out.write(read(2)); break;
	case code::Typecode::TYPE_U32:
	case code::Typecode::TYPE_I32: out.write(read(4)); break;
	case code::Typecode::TYPE_U64:
	case code::Typecode::TYPE_I64: out.write(read(8)); break;
	case code::Typecode::TYPE_F32: out.write(read(4)); break;
	case code::Typecode::TYPE_F64: out.write(read(8)); break;
	case code::Typecode::TYPE_ENUM:
	case code::Typecode::TYPE_STRING: out.write_value(read_string()); break;
	case code::Typecode::TYPE_LIST:{
		const auto size = read_le<code::length_t>();
		out.write_value(size);
		const auto &sub_type = type.sub_types.at(0);
		for(size_t i=0; i<size; i++){
			copy(out, sub_type);
		}
		break;
	}
	case code::Typecode::TYPE_MAP:{
		const auto size = read_le<code::length_t>();
		out.write_value(size);
		const auto &key_type = type.sub_types.at(0);
		const auto &value_type = type.sub_types.at(1);
		for(size_t i=0; i<size; i++){
			copy(out, key_type);
			copy(out, value_type);
		}
		break;
	}
	case code::Typecode::TYPE_PAIR:
		copy(out, type.sub_types.at(0));
		copy(out, type.sub_types.at(1));
		break;
	case code::Typecode::TYPE_TUPLE:{
		const auto size = type.sub_types.size();
		for(size_t i=0; i<size; i++){
			copy(out, type.sub_types.at(i));
		}
		break;
	}
	case code::Typecode::TYPE_DURATION: out.write(read(8)); break;
	case code::Typecode::TYPE_OPTIONAL:
	case code::Typecode::TYPE_POINTER:{
		const bool flag = read_bool();
		out.write_value(flag);
		if(flag){
			copy(out, type.sub_types.at(0));
		}
		break;
	}
	case code::Typecode::TYPE_VALUE:{
		const auto signature = read_signature();
		out.write_signature(signature);
		for(const auto &entry : signature.members){
			copy(out, entry.second);
		}
		break;
	}
	default:
		// suppress warning
		throw std::runtime_error("attempt to copy unknown type");
	}
}


bool TypeReader::read_bool(){
	return read_le<uint8_t>();
}


std::string TypeReader::read_string(){
	const auto size = read_le<code::length_t>();
	const auto chrs = read(size);
	std::string value;
	value.append(chrs.begin(), chrs.end());
	return value;
}


std::shared_ptr<data::Value> TypeReader::read_type_value(const code::Typecode &type){
	if(type.code != code::Typecode::TYPE_VALUE){
		skip(type);
		return nullptr;
	}
	const auto signature = read_signature();
	std::shared_ptr<data::Value> result;
	const auto chain = signature.lookup_chain();
	for(const auto &name : chain){
		const auto registry = core::lookup_type(name);
		if(registry.create){
			result = registry.create();
			break;
		}
	}
	if(!result){
		result = data::Value::create();
	}
	result->set_signature(signature);
	read_type(*result, signature);
	return result;
}


}
}
