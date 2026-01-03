#include <asgard/code/Signature.h>
#include <asgard/util/crc_64_xz.h>
#include <asgard/io/TypeWriter.h>
#include <asgard/io/VectorOutputSource.h>

#include <algorithm>


namespace asgard{
namespace code{


uint64_t Signature::hash() const{
	util::CRC_64_XZ crc;
	crc.update(name.c_str(), name.length());
	for(const auto &parent : parents){
		crc.update(parent.c_str(), parent.length());
	}
	for(const auto &entry : members){
		const auto &member_name = entry.first;
		const auto &type = entry.second;
		crc.update(member_name.c_str(), member_name.length());

		auto source = std::make_shared<io::VectorOutputSource>();
		{
			io::TypeWriter writer(source);
			writer.write_typecode(type);
		}
		const auto vector = source->get();
		crc.update(vector.data(), vector.size());
	}
	return crc.value();
}


std::vector<std::string> Signature::lookup_chain(bool include_this) const{
	std::vector<std::string> result = parents;
	if(include_this){
		result.push_back(name);
	}
	std::reverse(result.begin(), result.end());
	return result;
}


}
}
