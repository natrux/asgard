#include <asgard/data/Data.hxx>
#include <asgard/core/TypeRegistry.h>


namespace asgard{
namespace data{


code::Signature Data::static_signature(){
	auto sig = Super::static_signature();
	sig.name = "asgard.data.Data";
	return sig;
}


std::shared_ptr<Data> Data::create(){
	return std::make_shared<Data>();
}


code::Signature Data::signature() const{
	return static_signature();
}


void Data::read_member(io::TypeReader &reader, const std::string &name, const code::Typecode &code){
	Super::read_member(reader, name, code);
}


void Data::write_member(io::TypeWriter &writer, const std::string &name) const{
	Super::write_member(writer, name);
}


static core::register_type_t<Data> register_type;


}
}
