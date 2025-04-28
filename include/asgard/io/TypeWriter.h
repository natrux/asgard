#pragma once

#include <asgard/io/type_code_t.h>
#include <asgard/io/BufferedOutput.h>
#include <asgard/time/time.h>

#include <string>
#include <vector>
#include <array>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <optional>


namespace asgard{
namespace io{


class TypeWriter : public BufferedOutput{
public:
	TypeWriter(std::shared_ptr<OutputSource> source);

	void write_type(const std::nullptr_t &);
	void write_type(const bool &value);
	void write_type(const uint8_t &value);
	void write_type(const int8_t &value);
	void write_type(const uint16_t &value);
	void write_type(const int16_t &value);
	void write_type(const uint32_t &value);
	void write_type(const int32_t &value);
	void write_type(const uint64_t &value);
	void write_type(const int64_t &value);
	void write_type(const float &value);
	void write_type(const double &value);
	void write_type(const std::string &value);
	void write_type(const time::duration &value);
	void write_type(const time::time &value);
	void write_type(const time::wall_time &value);
	template<class T>
	void write_type(const std::vector<T> &value){
		write_list(value);
	}
	template<class T, size_t N>
	void write_type(const std::array<T, N> &value){
		write_list(value);
	}
	template<class T>
	void write_type(const std::set<T> &value){
		write_list(value);
	}
	template<class T>
	void write_type(const std::unordered_set<T> &value){
		write_list(value);
	}
	template<class K, class V>
	void write_type(const std::map<K, V> &value){
		write_map(value);
	}
	template<class K, class V>
	void write_type(const std::unordered_map<K, V> &value){
		write_map(value);
	}
	template<class T, class U>
	void write_type(const std::pair<T, U> &value){
		write_le(TYPE_PAIR);
		write_type(value.first);
		write_type(value.second);
	}
	template<class T>
	void write_type(const std::optional<T> &value){
		if(value){
			write_type(*value);
		}else{
			write_type(nullptr);
		}
	}
	template<class T>
	void write_type(std::shared_ptr<T> value){
		if(value){
			write_type(*value);
		}else{
			write_type(nullptr);
		}
	}

	template<class T>
	void write_type(const T &value){
		value.write_to(*this);
	}

private:
	template<class T>
	void write_le(const T &value){
		for(size_t i=0; i<sizeof(T); i++){
			const uint8_t byte = (value >> (i*8)) & 0xff;
			write(byte);
		}
	}
	template<class T>
	void write_list(const T &value){
		const uint64_t size = value.size();
		write_le(TYPE_LIST);
		write_le(size);
		for(const auto &entry : value){
			write_type(entry);
		}
	}
	template<class T>
	void write_map(const T &value){
		const uint64_t size = value.size();
		write_le(TYPE_MAP);
		write_le(size);
		for(const auto &entry : value){
			write_type(entry.first);
			write_type(entry.second);
		}
	}
};


}
}
