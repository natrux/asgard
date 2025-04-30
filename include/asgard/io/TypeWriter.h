#pragma once

#include <asgard/io/typecode_t.h>
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
	template<class... Ts>
	void write_type(const std::tuple<Ts...> &value){
		const uint64_t size = sizeof...(Ts);
		write_le(TYPE_TUPLE);
		write_le(size);
		write_tuple_elements<sizeof...(Ts)>(value);
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
		using B = util::uintw_t<T>;
		B tmp = 0;
		std::memcpy(&tmp, &value, sizeof(T));
		for(size_t i=0; i<sizeof(T); i++){
			const uint8_t byte = (tmp >> (i*8)) & 0xff;
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

	// N = remaining elements
	template<size_t N, class... Ts>
	typename std::enable_if<(N>0)>::type write_tuple_elements(const std::tuple<Ts...> &value){
		constexpr size_t I = sizeof...(Ts) - N;
		write_type(std::get<I>(value));
		write_tuple_elements<N-1>(value);
	}
	template<size_t N, class... Ts>
	typename std::enable_if<(N==0)>::type write_tuple_elements(const std::tuple<Ts...> &value){
	}
};


}
}
