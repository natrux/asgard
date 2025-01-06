#include <asgard/codegen/Namespace.h>
#include <asgard/codegen/Type.h>
#include <asgard/codegen/Declaration.h>
#include <asgard/codegen/Method.h>


namespace asgard{
namespace codegen{


void Namespace::insert(std::shared_ptr<Type> type){
	types.insert(type);
}


void Namespace::insert(std::shared_ptr<Type> type, const std::vector<std::string> &name_space){
	get(name_space).insert(type);
}


std::shared_ptr<Type> Namespace::find(const std::string &type_name) const{
	for(const auto &type : types){
		if(type->get_name() == type_name){
			return type;
		}
	}
	return nullptr;
}


std::shared_ptr<Type> Namespace::find(const std::string &type_name, const std::vector<std::string> &name_space) const{
	if(name_space.empty()){
		return find(type_name);
	}
	std::vector<std::string> sub_name_space(name_space.size()-1);
	std::copy(name_space.begin()+1, name_space.end(), sub_name_space.begin());
	const auto find = namespaces.find(name_space.front());
	if(find != namespaces.end()){
		return find->second.find(type_name, sub_name_space);
	}
	return nullptr;
}


bool Namespace::contains(const std::string &type_name) const{
	return (find(type_name) != nullptr);
}


std::set<std::shared_ptr<Type>> Namespace::get_types() const{
	std::set<std::shared_ptr<Type>> result;
	result.insert(types.begin(), types.end());
	for(const auto &sub : namespaces){
		const auto sub_types = sub.second.get_types();
		result.insert(sub_types.begin(), sub_types.end());
	}
	return result;
}


Namespace &Namespace::get(const std::string &name){
	return namespaces[name];
}


Namespace &Namespace::get(const std::vector<std::string> &names){
	if(names.empty()){
		return *this;
	}
	if(names.size() == 1){
		return get(names.front());
	}
	std::vector<std::string> sub_names(names.size()-1);
	std::copy(names.begin()+1, names.end(), sub_names.begin());
	return namespaces[names.front()].get(sub_names);
}


}
}
