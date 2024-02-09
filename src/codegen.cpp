#include <asgard/codegen/Namespace.h>
#include <asgard/codegen/DataType.h>
#include <asgard/codegen/ModuleType.h>
#include <asgard/codegen/Method.h>
#include <asgard/codegen/packages.h>
#include <asgard/codegen/Token.h>
#include <asgard/util/ls.h>

#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <fstream>


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
		std::shared_ptr<DataType> value = std::make_shared<DataType>("Value", name_space, "");
		value->set_builtin();
		root_namespace.insert(value, name_space);
	}
	{
		const std::vector<std::string> name_space = {"asgard", "mod"};
		std::shared_ptr<ModuleType> module = std::make_shared<ModuleType>("Module", name_space, "");
		module->set_builtin();
		root_namespace.insert(module, name_space);
	}
}


static void collect_types(const std::string &package_path, Namespace &root_namespace, const std::vector<std::string> &name_space){
	const std::string path = namespace_to_path(package_path, name_space);
	std::vector<std::string> files, directories;
	util::ls(path, files, directories);

	for(const auto &file : files){
		std::shared_ptr<Type> type;
		const auto dot = file.rfind('.');
		if(dot != std::string::npos){
			const std::string name = file.substr(0, dot);
			const std::string ext = file.substr(dot);
			if(ext == ext_declaration_data){
				type = std::make_shared<DataType>(name, name_space, package_path);
			}else if(ext == ext_declaration_module){
				type = std::make_shared<ModuleType>(name, name_space, package_path);
			}else if(ext == ext_declaration_enum){
				// TODO
			}
		}
		if(type){
			std::cout << "Found type: " << type->get_full_name(".") << std::endl;
			root_namespace.insert(type, name_space);
		}
	}
	for(const auto &dir : directories){
		std::vector<std::string> sub_name_space = name_space;
		sub_name_space.push_back(dir);
		collect_types(package_path, root_namespace, sub_name_space);
	}
}


static void codegen(const std::vector<std::string> &packages){
	Namespace root_namespace;

	add_builtins(root_namespace);

	for(const auto &package : packages){
		collect_types(package, root_namespace, {});
	}

	for(auto type : root_namespace.get_types()){
		if(type->get_builtin()){
			std::cout << "Builtin type: " << type->get_full_name(".")  << std::endl;
		}else{
			std::cout << "Parsing: " << type->get_full_name(".") << std::endl;

			std::string source;
			{
				const auto path = type->get_path_declaration();
				std::ifstream stream(path);
				if(!stream){
					throw std::runtime_error("Failed to open '" + path + "'");
				}
				source = std::string(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());
			}
			const auto tokens = tokenize(source);
			std::cout << "Tokenized source: ";
			for(const auto &token : tokens){
				std::cout << token.content << ", ";
			}
			std::cout << std::endl;
		}
	}

	for(const auto &type : root_namespace.get_types()){
		std::cout << "Generating: " << type->get_full_name(".") << std::endl;
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
