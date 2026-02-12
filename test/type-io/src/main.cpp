#include <asgard/io/TypeReader.h>
#include <asgard/io/TypeWriter.h>
#include <asgard/io/VectorInputSource.h>
#include <asgard/io/VectorOutputSource.h>
#include <asgard/data/Bin.h>
#include <asgard/time/time.h>
#include <asgard/time/strtime.h>

#include <asgard/data/DataPacket.hxx>
#include <asgard/data/log_level_e.hxx>

#include <asgard/type/io.h>

#include <iostream>
#include <iomanip>


template<class T>
std::vector<uint8_t> write_to_vector(const T &data_out){
	const auto t_w = asgard::time::now();
	const auto vector = asgard::type::write_to_vector(data_out);
	const auto d_w = asgard::time::since(t_w);

	const size_t max_output = 128;
	for(size_t i=0; i<std::min<size_t>(vector.size(), max_output); i++){
		const auto &byte = vector[i];
		std::cout << std::setw(2) << std::setfill('0') << std::hex << static_cast<unsigned int>(byte) << std::dec << " ";
	}
	if(vector.size() > max_output){
		std::cout << "...";
	}
	std::cout << std::endl;
	std::cout << vector.size() << " bytes written in " << asgard::time::strtime(d_w) << std::endl;

	return vector;
}


template<class T>
T read_from_vector(const std::vector<uint8_t> &vector){
	const auto t_r = asgard::time::now();
	const auto data_in = asgard::type::read_from_vector<typename std::remove_cv<typename std::remove_reference<T>::type>::type>(vector);
	const auto d_r = asgard::time::since(t_r);
	std::cout << vector.size() << " bytes read in " << asgard::time::strtime(d_r) << std::endl;

	return data_in;
}


template<class T>
void test_by_vector(const T &data_out){
	const auto vector = write_to_vector(data_out);
	const auto data_in = read_from_vector<T>(vector);

	const bool equal = (data_in == data_out);
	if(!equal){
		throw std::runtime_error("Not equal");
	}
}


template<class T>
void test_by_file(const T &data_out){
	const std::string path = "tmp.dat";
	asgard::type::write_to_file(data_out, path);
	const auto data_in = asgard::type::read_from_file<T>(path);

	const bool equal = (data_in == data_out);
	if(!equal){
		throw std::runtime_error("Not equal");
	}
}


template<class T>
void test_by_bin_io(const T &data_out){
	asgard::data::Bin value = data_out;
	typename std::remove_cv<typename std::remove_reference<T>::type>::type data_in;
	value.to(data_in);
	const bool equal = (data_in == data_out);
	if(!equal){
		throw std::runtime_error("Not equal");
	}
}


template<class T>
void test_by_bin_in(const T &data_out){
	const auto vector = write_to_vector(data_out);
	auto in = std::make_shared<asgard::io::VectorInputSource>(vector);
	asgard::data::Bin value;
	{
		asgard::io::TypeReader reader(in);
		reader.read_type(value);
	}
	typename std::remove_cv<typename std::remove_reference<T>::type>::type data_in;
	value.to(data_in);
	const bool equal = (data_in == data_out);
	if(!equal){
		throw std::runtime_error("Not equal");
	}
}


template<class T>
void test_by_bin_out(const T &data_out){
	asgard::data::Bin value = data_out;
	auto source = std::make_shared<asgard::io::VectorOutputSource>();
	{
		asgard::io::TypeWriter writer(source);
		writer.write_type(value);
	}
	const auto data_in = read_from_vector<T>(source->get());
	const bool equal = (data_in == data_out);
	if(!equal){
		throw std::runtime_error("Not equal");
	}
}


template<class T>
void test_by_bin(const T &data_out){
	test_by_bin_in(data_out);
	test_by_bin_out(data_out);
	test_by_bin_io(data_out);
}


template<class T>
void test_by(const T &data_out){
	test_by_vector(data_out);
	test_by_file(data_out);
	test_by_bin(data_out);
}


void test(){
	{
		const float number = 13.37;
		test_by(number);
	}

	{
		const std::map<std::tuple<std::string, int32_t>, std::set<double>> data = {
			{{"Hello", 42}, {23.42, 42.23}},
			{{"World", 23}, {-3.14, -2.71828}},
			{{"Foobar", -1337}, {-4.5, -1.9, 0, 1.9, 4.5}},
		};
		test_by(data);
	}

	{
		const std::vector<std::optional<int32_t>> data = {
			-42, {}, -23,
		};
		test_by(data);
	}

	{
		const std::vector<asgard::data::log_level_e> data = {
			asgard::data::log_level_e::DEBUG,
			asgard::data::log_level_e::INFO,
			asgard::data::log_level_e::WARN,
			asgard::data::log_level_e::ERROR,
		};
		test_by(data);
	}

	{
		const std::map<asgard::data::log_level_e, std::string> data = {
			{asgard::data::log_level_e::DEBUG, "debug log message"},
			{asgard::data::log_level_e::INFO, "informational log message"},
			{asgard::data::log_level_e::WARN, "warning log message"},
			{asgard::data::log_level_e::ERROR, "error log message"},
		};
		test_by(data);
	}

	{
		std::map<int16_t, asgard::data::DataPacket> data;
		for(int16_t i=0; i<10; i++){
			asgard::data::DataPacket packet;
			packet.time = asgard::time::now();
			for(uint8_t p=0; p<10; p++){
				packet.payload.push_back(0x11*i + p);
			}
			data[i] = packet;
		}
		test_by(data);
	}

	{
		const std::vector<std::string> letters = {"Alpha", "Beta", "Gamma", "Delta"};
		test_by(letters);
	}

	{
		std::vector<std::shared_ptr<asgard::data::DataPacket>> data;
		for(int16_t i=0; i<10; i++){
			auto packet = asgard::data::DataPacket::create();
			packet->time = asgard::time::now();
			for(uint8_t p=0; p<10; p++){
				packet->payload.push_back(0x11*i + p);
			}
			data.push_back(packet);
			if((i % 3) == 0){
				data.push_back(nullptr);
			}
		}

		asgard::type::write_to_file(data, "tmp.dat");
		const auto result = asgard::type::read_from_file<std::vector<std::shared_ptr<asgard::data::Value>>>("tmp.dat");

		const size_t size = data.size();
		if(result.size() != size){
			throw std::runtime_error("Not equal");
		}
		for(size_t i=0; i<size; i++){
			const auto &d1 = data[i];
			const auto &d2 = result[i];
			if(!d1 && !d2){
				continue;
			}
			if(!d1 || !d2){
				throw std::runtime_error("Not equal");
			}
			auto p1 = std::dynamic_pointer_cast<const asgard::data::DataPacket>(d1);
			auto p2 = std::dynamic_pointer_cast<const asgard::data::DataPacket>(d2);
			if(!p1 || !p1 || !(*p1 == *p2)){
				throw std::runtime_error("Not equal");
			}
		}
	}
}


int main(int, char **){
	try{
		test();
	}catch(const std::exception &err){
		std::cerr << "Error: " << err.what() << std::endl;
	}

	return 0;
}

