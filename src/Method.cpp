#include <asgard/codegen/Method.h>

#include <sstream>


namespace asgard{
namespace codegen{


void Method::to_string(std::ostream &stream) const{
	if(is_pure_virtual){
		stream << "virtual ";
	}
	if(output.empty()){
		stream << "void";
	}else if(output.size() == 1){
		stream << output[0].to_string();
	}else{
		// TODO tuple?
		throw std::logic_error("Not implemented");
	}
	stream << " ";
	stream << name;
	stream << "(";
	{
		bool is_first = true;
		for(const auto &d : input){
			if(!is_first){
				stream << ", ";
			}
			d.to_string(stream);
			is_first = false;
		}
	}
	stream << ")";
	if(is_const){
		stream << " const";
	}
	if(is_override){
		stream << " override";
	}
	if(is_pure_virtual){
		stream << " = 0";
	}
}


std::string Method::to_string() const{
	std::stringstream stream;
	to_string(stream);
	return stream.str();
}


}
}