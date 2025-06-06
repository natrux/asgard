#pragma once

#include <asgard/io/typecode_t.h>
#include <asgard/io/get_typecode.h>
#include <asgard/io/BufferedOutput.h>
#include <asgard/time/time.h>
#include <asgard/util/uintw_t.h>

#include <string>
#include <vector>
#include <array>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <optional>
#include <cstring>


namespace asgard{
namespace io{


class TypeWriter : public BufferedOutput{
public:
	TypeWriter(std::shared_ptr<OutputSource> source);

	void write_typecode(const typecode_t &type);

	template<class T>
	void write_type(const T &value){
		const auto type = get_typecode<T>();
		write_typecode(type);
		write_value(value);
	}

	void write_type(const char *value){
		// special case for string literals
		const std::string tmp = value;
		write_type(tmp);
	}

	void write_value(const std::nullptr_t &);
	void write_value(const bool &value);
	void write_value(const uint8_t &value);
	void write_value(const int8_t &value);
	void write_value(const uint16_t &value);
	void write_value(const int16_t &value);
	void write_value(const uint32_t &value);
	void write_value(const int32_t &value);
	void write_value(const uint64_t &value);
	void write_value(const int64_t &value);
	void write_value(const float &value);
	void write_value(const double &value);
	void write_value(const std::string &value);
	void write_value(const char *value);
	void write_value(const time::duration &value);
	void write_value(const time::time &value);
	void write_value(const time::wall_time &value);
	template<class T>
	void write_value(const std::vector<T> &value){
		write_list(value);
	}
	template<class T, size_t N>
	void write_value(const std::array<T, N> &value){
		write_list(value);
	}
	template<class T>
	void write_value(const std::set<T> &value){
		write_list(value);
	}
	template<class T>
	void write_value(const std::unordered_set<T> &value){
		write_list(value);
	}
	template<class K, class V>
	void write_value(const std::map<K, V> &value){
		write_map(value);
	}
	template<class K, class V>
	void write_value(const std::unordered_map<K, V> &value){
		write_map(value);
	}
	template<class T, class U>
	void write_value(const std::pair<T, U> &value){
		write_value(value.first);
		write_value(value.second);
	}
	template<class... Ts>
	void write_value(const std::tuple<Ts...> &value){
		write_tuple_elements<sizeof...(Ts)>(value);
	}
	template<class T>
	void write_value(const std::optional<T> &value){
		const uint8_t flag = static_cast<bool>(value);
		write_le(flag);
		if(flag){
			write_value(*value);
		}
	}
	template<class T>
	void write_value(const std::shared_ptr<T> &value){
		const uint8_t flag = static_cast<bool>(value);
		write_le(flag);
		if(flag){
			write_value(*value);
		}
	}

	template<class T>
	void write_le(const T &value){
		using B = util::uintw_t<T>;
		B tmp = 0;
		std::memcpy(&tmp, &value, sizeof(T));
		for(size_t i=0; i<sizeof(T); i++){
			const uint8_t byte = (tmp >> (i*8)) & 0xff;
			write(byte);
		}
	}

private:
	template<class T>
	void write_list(const T &value){
		const length_t size = value.size();
		write_le(size);
		for(const auto &entry : value){
			write_value(entry);
		}
	}

	template<class T>
	void write_map(const T &value){
		const length_t size = value.size();
		write_le(size);
		for(const auto &entry : value){
			write_value(entry.first);
			write_value(entry.second);
		}
	}

	// N = remaining elements
	template<size_t N, class... Ts>
	typename std::enable_if<(N>0)>::type write_tuple_elements(const std::tuple<Ts...> &value){
		constexpr size_t I = sizeof...(Ts) - N;
		write_value(std::get<I>(value));
		write_tuple_elements<N-1>(value);
	}
	template<size_t N, class... Ts>
	typename std::enable_if<(N==0)>::type write_tuple_elements(const std::tuple<Ts...> &value){
	}
};


}
}
