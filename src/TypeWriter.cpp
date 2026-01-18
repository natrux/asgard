#include <asgard/io/TypeWriter.h>
#include <asgard/data/Value.h>
#include <asgard/data/Bin.h>

#include <stdexcept>
#include <cstring>


namespace asgard{
namespace io{


TypeWriter::TypeWriter(std::shared_ptr<OutputSource> source):
	BufferedOutput(source)
{
}


void TypeWriter::write_typecode(const code::Typecode &type){
	code::length_t sub_types = 0;
	bool write_size = false;
	switch(type.code){
	case code::Typecode::TYPE_LIST:
	case code::Typecode::TYPE_OPTIONAL:
	case code::Typecode::TYPE_POINTER:
		sub_types = 1;
		break;
	case code::Typecode::TYPE_MAP:
	case code::Typecode::TYPE_PAIR:
		sub_types = 2;
		break;
	case code::Typecode::TYPE_TUPLE:
		sub_types = type.sub_types.size();
		write_size = true;
		break;
	default:
		sub_types = 0;
		break;
	}
	sub_types = std::min<code::length_t>(sub_types, type.sub_types.size());

	write_le(type.code);
	if(write_size){
		write_le(sub_types);
	}
	for(size_t i=0; i<sub_types; i++){
		write_typecode(type.sub_types.at(i));
	}
}


void TypeWriter::write_signature(const code::Signature &signature){
	const auto id = core::ID(signature.hash());
	const bool written = (signatures.find(id) != signatures.end());

	write_value(static_cast<uint64_t>(id));
	write_value(!written);
	if(!written){
		write_value(signature.name);

		const code::length_t num_parents = signature.parents.size();
		write_value(num_parents);
		for(const auto &parent : signature.parents){
			write_value(parent);
		}

		const code::length_t num_members = signature.members.size();
		write_value(num_members);
		for(const auto &entry : signature.members){
			write_value(entry.first);
			write_typecode(entry.second);
		}

		signatures.insert(id);
	}
}


void TypeWriter::write_enum_map(const code::EnumMap &map){
	const auto id = core::ID(map.hash());
	const bool written = (enum_maps.find(id) != enum_maps.end());

	write_value(static_cast<uint64_t>(id));
	write_value(!written);
	if(!written){
		write_value(map.name);

		const code::length_t num_values = map.enum_map.size();
		write_value(num_values);
		for(const auto &entry : map.enum_map){
			write_value(entry.first);
			write_value(entry.second);
		}

		enum_maps.insert(id);
	}
}


void TypeWriter::write_type(const data::Bin &value){
	auto source = std::make_shared<io::VectorInputSource>(value.get_data());
	TypeReader reader(source);
	reader.copy(*this);
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
	const code::length_t length = value.size();
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


void TypeWriter::write_value(const data::Value &value){
	const auto signature = value.signature();
	write_signature(signature);
	for(const auto &entry : signature.members){
		try{
			value.write_member(*this, entry.first);
		}catch(const std::logic_error &err){
			write_empty_value(entry.second);
		}
	}
}


void TypeWriter::write_value(const data::Enum &value){
	write_value(value.to_string());
}


void TypeWriter::write_value(const data::Bin &value){
	auto source = std::make_shared<io::VectorInputSource>(value.get_data());
	TypeReader reader(source);
	const auto type = reader.read_typecode();
	reader.copy(*this, type);
}


void TypeWriter::write_empty_value(const code::Typecode &type){
	switch(type.code){
	case code::Typecode::TYPE_NULL:
		break;
	case code::Typecode::TYPE_BOOL:
		write_value(false);
		break;
	case code::Typecode::TYPE_U8:
		write_value(static_cast<uint8_t>(0));
		break;
	case code::Typecode::TYPE_I8:
		write_value(static_cast<int8_t>(0));
		break;
	case code::Typecode::TYPE_U16:
		write_value(static_cast<uint16_t>(0));
		break;
	case code::Typecode::TYPE_I16:
		write_value(static_cast<int16_t>(0));
		break;
	case code::Typecode::TYPE_U32:
		write_value(static_cast<uint32_t>(0));
		break;
	case code::Typecode::TYPE_I32:
		write_value(static_cast<int32_t>(0));
		break;
	case code::Typecode::TYPE_U64:
		write_value(static_cast<uint64_t>(0));
		break;
	case code::Typecode::TYPE_I64:
		write_value(static_cast<int64_t>(0));
		break;
	case code::Typecode::TYPE_F32:
		write_value(static_cast<float>(0));
		break;
	case code::Typecode::TYPE_F64:
		write_value(static_cast<double>(0));
		break;
	case code::Typecode::TYPE_LIST:
		write_value(std::vector<uint8_t>());
		break;
	case code::Typecode::TYPE_MAP:
		write_value(std::map<uint8_t, uint8_t>());
		break;
	case code::Typecode::TYPE_PAIR:
		write_empty_value(type.sub_types.at(0));
		write_empty_value(type.sub_types.at(1));
		break;
	case code::Typecode::TYPE_TUPLE:
		for(const auto &sub_type : type.sub_types){
			write_empty_value(sub_type);
		}
		break;
	case code::Typecode::TYPE_DURATION:
		write_value(time::immediate());
		break;
	case code::Typecode::TYPE_STRING:
		write_value("");
		break;
	case code::Typecode::TYPE_OPTIONAL:
		write_value(std::optional<uint8_t>());
		break;
	case code::Typecode::TYPE_POINTER:
		write_value(std::shared_ptr<const uint8_t>());
		break;
	case code::Typecode::TYPE_VALUE:
		write_signature(code::Signature());
		break;
	case code::Typecode::TYPE_ENUM:
		write_value("");
		break;
	default:
		break;
	}
}


}
}
