#include <asgard/core/TypeRegistry.h>

#include <map>
#include <mutex>


namespace asgard{
namespace core{


class TypeRegistry{
public:
	void register_type(const type_register_t &reg){
		std::lock_guard<std::mutex> lock(mutex);
		types[reg.signature.name] = reg;
	}

	type_register_t lookup_type(const ID &id){
		std::lock_guard<std::mutex> lock(mutex);
		const auto find = types.find(id);
		if(find != types.end()){
			return find->second;
		}
		return type_register_t();
	}

private:
	std::mutex mutex;
	std::map<ID, type_register_t> types;
};


static TypeRegistry &get_registry(){
	static TypeRegistry registry;
	return registry;
}


void register_type(const type_register_t &reg){
	get_registry().register_type(reg);
}


type_register_t lookup_type(const ID &id){
	return get_registry().lookup_type(id);
}


}
}
