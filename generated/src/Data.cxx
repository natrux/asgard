#include <asgard/data/Data.hxx>


namespace asgard{
namespace data{


code::Signature Data::static_signature(){
	auto sig = Super::static_signature();
	sig.name = "asgard.data.Data";
	return sig;
}


code::Signature Data::signature() const{
	return static_signature();
}


bool Data::try_read_member(io::TypeReader &reader, const std::string &name){
	if(Super::try_read_member(reader, name)){
		return true;
	}
	return false;
}


bool Data::try_write_member(io::TypeWriter &writer, const std::string &name) const{
	if(Super::try_write_member(writer, name)){
		return true;
	}
	return false;
}


}
}
