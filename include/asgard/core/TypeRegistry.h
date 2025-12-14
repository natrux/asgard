#pragma once

#include <asgard/code/Signature.h>
#include <asgard/data/Value.h>
#include <asgard/core/ID.h>

#include <functional>


namespace asgard{
namespace core{


struct type_register_t{
	code::Signature signature;
	std::function<std::shared_ptr<data::Value>()> create;
};


void register_type(const type_register_t &reg);
type_register_t lookup_type(const core::ID &id);


template<class T>
struct register_type_t{
	register_type_t(){
		type_register_t type;
		type.signature = T::static_signature();
		type.create = T::create;
		register_type(type);
	}
};


}
}
