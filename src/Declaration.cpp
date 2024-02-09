#include <asgard/codegen/Declaration.h>
#include <asgard/codegen/DataType.h>
#include <asgard/codegen/Method.h>

#include <sstream>


namespace asgard{
namespace codegen{


void Declaration::to_string(std::ostream &stream) const{
	if(is_const){
		stream << "const ";
	}
	stream << type->get_name();
	if(!template_types.empty()){
		stream << "<";
		bool is_first = true;
		for(const auto &t : template_types){
			if(!is_first){
				stream << ", ";
			}
			stream << t->get_name();
			is_first = false;
		}
		stream << ">";
	}
	stream << " ";
	if(is_reference){
		stream << "&";
	}
	stream << name;
	if(initializer){
		stream << " = " << *initializer;
	}else if(type->has_default_initializer()){
		stream << " = " << type->get_default_initializer();
	}
}


std::string Declaration::to_string() const{
	std::stringstream stream;
	to_string(stream);
	return stream.str();
}


}
}