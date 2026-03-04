#include <asgard/data/Message.hxx>
#include <asgard/core/TypeRegistry.h>


namespace asgard{
namespace data{


code::Signature Message::static_signature(){
	auto sig = Super::static_signature();
	sig.parents.push_back(sig.name);
	sig.name = "asgard.data.Message";
	return sig;
}


std::shared_ptr<Message> Message::create(){
	return std::make_shared<Message>();
}


Message::Message(){
	set_signature(static_signature());
}


void Message::read_member(type::TypeReader &reader, const std::string &name, const code::Typecode &type){
	Super::read_member(reader, name, type);
}


void Message::write_member(type::TypeWriter &writer, const std::string &name) const{
	Super::write_member(writer, name);
}


void Message::set_member(const std::string &name, const Bin &value){
	Super::set_member(name, value);
}


Bin Message::get_member(const std::string &name) const{
	return Super::get_member(name);
}


static core::register_type_t<Message> register_type;


}
}
