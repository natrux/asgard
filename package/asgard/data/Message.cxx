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


void Message::read_member(io::TypeReader &reader, const std::string &name, const code::Typecode &type){
	Super::read_member(reader, name, type);
}


void Message::write_member(io::TypeWriter &writer, const std::string &name) const{
	Super::write_member(writer, name);
}


static core::register_type_t<Message> register_type;


}
}
