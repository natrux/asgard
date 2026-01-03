#include <asgard/data/Data.hxx>
#include <asgard/core/TypeRegistry.h>


namespace asgard{
namespace data{


code::Signature Data::static_signature(){
	auto sig = Super::static_signature();
	sig.parents.push_back(sig.name);
	sig.name = "asgard.data.Data";
	return sig;
}


std::shared_ptr<Data> Data::create(){
	return std::make_shared<Data>();
}


Data::Data(){
	set_signature(static_signature());
}


void Data::read_member(io::TypeReader &reader, const std::string &name, const code::Typecode &type){
	Super::read_member(reader, name, type);
}


void Data::write_member(io::TypeWriter &writer, const std::string &name) const{
	Super::write_member(writer, name);
}


static core::register_type_t<Data> register_type;


}
}
