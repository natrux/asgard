#include <asgard/codegen/Method.h>

#include <sstream>


namespace asgard{
namespace codegen{


std::string Method::to_string() const{
	std::stringstream  stream;

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
		std::vector<std::string> input_str;
		for(const auto &d : input){
			input_str.push_back(d.to_string());
		}
		for(size_t i=0; i<input_str.size(); i++){
			if(i > 0){
				stream << ", ";
			}
			stream << input_str[i];
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

	return stream.str();
}


}
}