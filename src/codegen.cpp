#include <asgard/codegen/Namespace.h>
#include <asgard/codegen/DataType.h>
#include <asgard/codegen/ModuleType.h>
#include <asgard/codegen/Method.h>
#include <asgard/codegen/packages.h>
#include <asgard/util/ls.h>

#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>


namespace asgard{
namespace codegen{


static void add_builtin_type(Namespace &root_namespace, const std::string &name, const std::vector<std::string> &name_space, const std::string &init){
	std::shared_ptr<DataType> data = std::make_shared<DataType>(name, name_space, "");
	data->set_default_initializer(init);
	data->set_builtin();
	root_namespace.insert(data, name_space);
}


static void add_builtins(Namespace &root_namespace){
	add_builtin_type(root_namespace, "u8", {}, "0");
	add_builtin_type(root_namespace, "i8", {}, "0");
	add_builtin_type(root_namespace, "u16", {}, "0");  // uint16_t @ cstdint
	add_builtin_type(root_namespace, "i16", {}, "0");
	add_builtin_type(root_namespace, "u32", {}, "0");
	add_builtin_type(root_namespace, "i32", {}, "0");
	add_builtin_type(root_namespace, "u64", {}, "0");
	add_builtin_type(root_namespace, "i64", {}, "0");
	add_builtin_type(root_namespace, "size_t", {}, "0");  // std::size_t @ cstddef
	add_builtin_type(root_namespace, "f32", {}, "0.0");
	add_builtin_type(root_namespace, "f64", {}, "0.0");
	add_builtin_type(root_namespace, "bool", {}, "false");
	add_builtin_type(root_namespace, "string", {}, "\"\"");   // std::string @ string
	// vector, map, ...

	{
		const std::vector<std::string> name_space = {"asgard", "data"};

		std::shared_ptr<DataType> data = std::make_shared<DataType>("Data", name_space, "");
		data->set_builtin();
		root_namespace.insert(data, name_space);

		std::shared_ptr<DataType> value = std::make_shared<DataType>("Value", name_space, "");
		value->set_builtin();
		value->set_parent(data);
		root_namespace.insert(value, name_space);
	}
	{
		const std::vector<std::string> name_space = {"asgard", "mod"};
		std::shared_ptr<ModuleType> module = std::make_shared<ModuleType>("Module", name_space, "");
		module->set_builtin();
		root_namespace.insert(module, name_space);
	}
}


static void collect_types(const std::string &package_path, Namespace &root_namespace, const package_content_e &content_type, const std::vector<std::string> &name_space){
	const std::string path = namespace_to_path(package_path, content_type, name_space);
	std::vector<std::string> files, directories;
	util::ls(path, files, directories);

	for(const auto &file : files){
		const std::string file_path = path + "/" + file;
		std::shared_ptr<Type> type;
		if(content_type == package_content_e::DATA){
			type = std::make_shared<DataType>(file, name_space, package_path);
		}else if(content_type == package_content_e::MODULES){
			type = std::make_shared<ModuleType>(file, name_space, package_path);
		}
		if(type){
			std::cout << "Found type: " << type->get_full_name(".") << std::endl;
			root_namespace.insert(type, name_space);
		}
	}
	for(const auto &dir : directories){
		std::vector<std::string> sub_name_space = name_space;
		sub_name_space.push_back(dir);
		collect_types(package_path, root_namespace, content_type, sub_name_space);
	}
}


static void codegen(const std::vector<std::string> &packages){
	Namespace root_namespace;

	add_builtins(root_namespace);

	for(const auto &package : packages){
		const std::string path = package + "/" + dir_package;
		std::vector<std::string> files, directories;
		try{
			util::ls(path, files, directories);
		}catch(const std::runtime_error &err){
			std::cerr << "Skipping package " << package << " with error: " << err.what() << std::endl;
			continue;
		}

		for(const auto &dir : directories){
			package_content_e content = package_content_e::NONE;
			if(dir == dir_data){
				content = package_content_e::DATA;
			}else if(dir == dir_modules){
				content = package_content_e::MODULES;
			}
			if((content == package_content_e::DATA || content == package_content_e::MODULES)){
				const std::string root_path = path + "/" + dir;
				collect_types(package, root_namespace, content, {});
			}
		}
	}

	for(auto type : root_namespace.get_types()){
		if(type->get_builtin()){
			std::cout << "Builtin type: " << type->get_full_name(".")  << std::endl;
		}else{
			std::cout << "Parsing: " << type->get_full_name(".") << std::endl;
			type->parse(root_namespace);
		}
	}

	for(const auto &type : root_namespace.get_types()){
		std::cout << "Generating: " << type->get_full_name(".") << std::endl;
		type->generate_code();
	}
}



}
}


int main(int argc, char **argv){
	std::vector<std::string> packages;
	for(int i=1; i<argc; i++){
		packages.push_back(argv[i]);
	}

	try{
		asgard::codegen::codegen(packages);
	}catch(const std::exception &err){
		std::cerr << "codegen failed with: " << err.what() << std::endl;
		return 1;
	}

	return 0;
}
