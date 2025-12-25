#pragma once

#include <asgard/io/TypeReader.h>
#include <asgard/io/TypeWriter.h>
#include <asgard/io/VectorInputSource.h>
#include <asgard/io/VectorOutputSource.h>


namespace asgard{
namespace data{


class Bin{
public:
	template<class T>
	Bin(const T &value){
		*this = value;
	}

	Bin();

	template<class T>
	Bin &operator=(const T &value){
		auto source = std::make_shared<io::VectorOutputSource>();
		{
			io::TypeWriter writer(source);
			writer.write_type(value);
		}
		data = source->get();
		update();
		return *this;
	}

	std::vector<uint8_t> get_data() const;
	void set_data(const std::vector<uint8_t> &data);

	template<class T>
	void to(T &value) const{
		auto source = std::make_shared<io::VectorInputSource>(data);
		io::TypeReader reader(source);
		reader.read_type(value);
	}

	template<class T>
	T to() const{
		T value;
		to(value);
		return value;
	}

	bool is_empty() const;
	bool is_bool() const;
	bool is_integer() const;
	bool is_unsigned_integer() const;
	bool is_floating_point() const;
	bool is_string() const;
	bool is_list() const;
	bool is_map() const;

private:
	std::vector<uint8_t> data;
	code::Typecode type;
	code::Signature signature;

	void update();
};


}
}
