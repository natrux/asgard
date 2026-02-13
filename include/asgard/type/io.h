#pragma once

#include <asgard/fs/File.h>


namespace asgard{
namespace type{


template<class T>
void write_to_file(const T &value, const std::string &path){
	const fs::File file(path);
	auto source = file.output_source();
	TypeWriter writer(source);
	writer.write_type(value);
}


template<class T>
void read_from_file(T &value, const std::string &path){
	const fs::File file(path);
	auto source = file.input_source();
	TypeReader reader(source);
	reader.read_type(value);
}


template<class T>
T read_from_file(const std::string &path){
	T value;
	read_from_file(value, path);
	return value;
}


template<class T>
std::vector<uint8_t> write_to_vector(const T &value){
	auto source = std::make_shared<io::VectorOutputSource>();
	{
		TypeWriter writer(source);
		writer.write_type(value);
	}
	return source->get();
}


template<class T>
void read_from_vector(T &value, const std::vector<uint8_t> &vector){
	auto source = std::make_shared<asgard::io::VectorInputSource>(vector);
	TypeReader reader(source);
	reader.read_type(value);
}


template<class T>
T read_from_vector(const std::vector<uint8_t> &vector){
	T value;
	read_from_vector(value, vector);
	return value;
}


}
}
