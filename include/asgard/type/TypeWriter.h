#pragma once

#include <asgard/code/Typecode.h>
#include <asgard/code/get_typecode.h>
#include <asgard/code/Signature.h>
#include <asgard/code/EnumMap.h>
#include <asgard/io/BufferedOutput.h>
#include <asgard/time/time.h>
#include <asgard/core/ID.h>
#include <asgard/data/Enum.h>
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
namespace data{
class Bin;
class Value;
}
}

namespace asgard{
namespace type{


class TypeWriter : public io::BufferedOutput{
public:
	TypeWriter(std::shared_ptr<io::OutputSource> source);

	void write_typecode(const code::Typecode &type);
	void write_signature(const code::Signature &signature);
	void write_enum_map(const code::EnumMap &map);

	template<class T>
	void write_type(const T &value){
		const auto type = code::get_typecode<T>();
		write_typecode(type);
		write_value(value);
	}

	void write_type(const char *value){
		// special case for string literals
		const std::string tmp = value;
		write_type(tmp);
	}

	void write_type(const data::Bin &value);

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
	void write_value(const data::Value &value);
	void write_value(const data::Enum &value);
	void write_value(const data::Bin &value);

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

	void write_empty_value(const code::Typecode &type);

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
	std::set<core::ID> signatures;
	std::set<core::ID> enum_maps;

	template<class T>
	void write_list(const T &value){
		const code::length_t size = value.size();
		write_le(size);
		for(const auto &entry : value){
			write_value(entry);
		}
	}

	template<class T>
	void write_map(const T &value){
		const code::length_t size = value.size();
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
