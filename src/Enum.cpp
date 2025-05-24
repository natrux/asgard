#include <asgard/data/Enum.h>
#include <asgard/io/TypeReader.h>
#include <asgard/io/TypeWriter.h>


namespace asgard{
namespace data{


void Enum::read_from(io::TypeReader &reader, io::typecode_e code){
	if(code == io::typecode_t::TYPE_ENUM){
		const auto size = reader.read_le<uint64_t>();
		const auto chrs = reader.read(size);
		const std::string str(chrs.begin(), chrs.end());
		from_string(str);
	}else if(code == io::typecode_t::TYPE_STRING){
		std::string str;
		reader.read_type(str, code);
		from_string(str);
	}else{
		reader.skip(code);
	}
}


void Enum::write_to(io::TypeWriter &writer) const{
	const std::string str = to_string();
	const uint64_t size = str.size();
	writer.write_le(io::typecode_t::TYPE_ENUM);
	writer.write_le(size);
	for(const auto &chr : str){
		writer.write(static_cast<uint8_t>(chr));
	}
}


}
}
