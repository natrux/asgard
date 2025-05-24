#include <asgard/io/TypeReader.h>
#include <asgard/io/TypeWriter.h>
#include <asgard/io/VectorInputSource.h>
#include <asgard/io/VectorOutputSource.h>
#include <asgard/time/time.h>
#include <asgard/time/strtime.h>

#include <asgard/data/DataPacket.hxx>
#include <asgard/data/log_level_e.hxx>

#include <iostream>
#include <iomanip>


template<class T>
void test_io(const T &data_out){
	typename std::remove_cv<typename std::remove_reference<T>::type>::type data_in;

	auto out = std::make_shared<asgard::io::VectorOutputSource>();
	const auto t_w = asgard::time::now();
	{
		asgard::io::TypeWriter writer(out);
		writer.write_type(data_out);
	}
	const auto d_w = asgard::time::since(t_w);

	const auto vector = out->get();
	auto in = std::make_shared<asgard::io::VectorInputSource>(vector);
	const auto t_r = asgard::time::now();
	{
		asgard::io::TypeReader reader(in);
		reader.read_type(data_in);
	}
	const auto d_r = asgard::time::since(t_r);

	const size_t max_output = 128;
	for(size_t i=0; i<std::min<size_t>(vector.size(), max_output); i++){
		const auto &byte = vector[i];
		std::cout << std::setw(2) << std::setfill('0') << std::hex << static_cast<unsigned int>(byte) << std::dec << " ";
	}
	if(vector.size() > max_output){
		std::cout << "...";
	}
	std::cout << std::endl;
	std::cout << vector.size() << " bytes w/r in " << asgard::time::strtime(d_w) << " / " << asgard::time::strtime(d_r) << std::endl;

	const bool equal = (data_in == data_out);
	if(equal){
		std::cout << "Equal" << std::endl;
	}else{
		std::cout << "Not equal" << std::endl;
	}
}


int main(int, char **){
	{
		const float number = 13.37;
		test_io(number);
	}

	{
		const std::map<std::tuple<std::string, int32_t>, std::set<double>> data = {
			{{"Hello", 42}, {23.42, 42.23}},
			{{"World", 23}, {-3.14, -2.71828}},
			{{"Foobar", -1337}, {-4.5, -1.9, 0, 1.9, 4.5}},
		};
		test_io(data);
	}

	{
		std::vector<asgard::data::DataPacket> packet_list;
		for(size_t i=0; i<5; i++){
			asgard::data::DataPacket packet;
			packet.time = asgard::time::now();
			packet.payload = {0, 255, 23, 42, 0x23, 0x42, static_cast<uint8_t>(i)};
			packet_list.push_back(packet);
		}
		test_io(packet_list);
	}

	{
		const asgard::data::log_level_e level = asgard::data::log_level_e::INFO;
		test_io(level);
	}

	return 0;
}
