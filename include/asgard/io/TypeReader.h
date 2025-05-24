#pragma once

#include <asgard/io/BufferedInput.h>
#include <asgard/io/typecode_t.h>
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


class TypeReader : public BufferedInput{
public:
	TypeReader(std::shared_ptr<InputSource> source);

	time::duration get_delta_time() const;
	void set_delta_time(const time::duration &delta);

	template<class T>
	T read_type(){
		T value;
		read_type(value);
		return value;
	}

	template<class T>
	void read_type(T &value){
		const auto code = read_le<typecode_e>();
		read_type(value, code);
	}

	void read_type(bool &value, typecode_e code);
	void read_type(uint8_t &value, typecode_e code);
	void read_type(int8_t &value, typecode_e code);
	void read_type(uint16_t &value, typecode_e code);
	void read_type(int16_t &value, typecode_e code);
	void read_type(uint32_t &value, typecode_e code);
	void read_type(int32_t &value, typecode_e code);
	void read_type(uint64_t &value, typecode_e code);
	void read_type(int64_t &value, typecode_e code);
	void read_type(float &value, typecode_e code);
	void read_type(double &value, typecode_e code);
	void read_type(std::string &value, typecode_e code);
	void read_type(time::time &value, typecode_e code);
	void read_type(time::wall_time &value, typecode_e code);
	void read_type(time::duration &value, typecode_e code);

	template<class T>
	void read_type(std::vector<T> &value, typecode_e code){
		if(code == typecode_t::TYPE_LIST){
			const auto size = read_le<uint64_t>();
			for(size_t i=0; i<size; i++){
				value.push_back(read_type<T>());
			}
		}else if(code == typecode_t::TYPE_PAIR){
			value.push_back(read_type<T>());
			value.push_back(read_type<T>());
		}else if(code == typecode_t::TYPE_TUPLE){
			const auto size = read_le<uint64_t>();
			for(size_t i=0; i<size; i++){
				value.push_back(read_type<T>());
			}
		}else{
			skip(code);
		}
	}

	template<class T, size_t N>
	void read_type(std::array<T, N> &value, typecode_e code){
		if(code == typecode_t::TYPE_LIST){
			const auto size = read_le<uint64_t>();
			for(size_t i=0; i<size; i++){
				if(i < N){
					value[i] = read_type<T>();
				}else{
					skip();
				}
			}
		}else if(code == typecode_t::TYPE_PAIR){
			if(N >= 2){
				value[0] = read_type<T>();
				value[1] = read_type<T>();
			}else if(N >= 1){
				value[0] = read_type<T>();
				skip();
			}else{
				skip();
				skip();
			}
		}else if(code == typecode_t::TYPE_TUPLE){
			const auto size = read_le<uint64_t>();
			for(size_t i=0; i<size; i++){
				if(i < N){
					value[i] = read_type<T>();
				}else{
					skip();
				}
			}
		}else{
			skip(code);
		}
	}

	template<class T>
	void read_type(std::set<T> &value, typecode_e code){
		read_set(value, code);
	}

	template<class T>
	void read_type(std::unordered_set<T> &value, typecode_e code){
		read_set(value, code);
	}

	template<class K, class V>
	void read_type(std::map<K, V> &value, typecode_e code){
		read_map(value, code);
	}

	template<class K, class V>
	void read_type(std::unordered_map<K, V> &value, typecode_e code){
		read_map(value, code);
	}

	template<class T, class U>
	void read_type(std::pair<T, U> &value, typecode_e code){
		if(code == typecode_t::TYPE_PAIR){
			value.first = read_type<T>();
			value.second = read_type<U>();
		}else if(code == typecode_t::TYPE_LIST){
			const auto size = read_le<uint64_t>();
			if(size >= 1){
				value.first = read_type<T>();
			}
			if(size >= 2){
				value.second = read_type<U>();
			}
			for(size_t i=2; i<size; i++){
				skip();
			}
		}else{
			skip(code);
		}
	}

	template<class... Ts>
	void read_type(std::tuple<Ts...> &value, typecode_e code){
		constexpr size_t N = sizeof...(Ts);
		if(code == typecode_t::TYPE_TUPLE){
			const auto size = read_le<uint64_t>();
			read_tuple_elements<N>(value, size);
			for(size_t i=N; i<size; i++){
				skip();
			}
		}else if(code == typecode_t::TYPE_PAIR){
			read_tuple_elements<N>(value, 2);
			for(size_t i=N; i<2; i++){
				skip();
			}
		}else if(code == typecode_t::TYPE_LIST){
			const auto size = read_le<uint64_t>();
			read_tuple_elements<N>(value, size);
			for(size_t i=N; i<size; i++){
				skip();
			}
		}else{
			skip(code);
		}
	}

	template<class T>
	void read_type(std::optional<T> &value, typecode_e code){
		if(code == typecode_t::TYPE_NULL){
			return;
		}
		T optional;
		read_type(optional, code);
		value = optional;
	}

	template<class T>
	void read_type(std::shared_ptr<T> &value, typecode_e code){
		if(code == typecode_t::TYPE_NULL){
			return;
		}
		auto ptr = std::make_shared<T>();
		read_type(*ptr, code);
		value = ptr;
	}

	template<class T>
	void read_type(T &value, typecode_e code){
		value.read_from(*this, code);
	}

	void skip();
	void skip(typecode_e code);

	template<class T>
	T read_le(){
		using B = util::uintw_t<T>;
		B tmp = 0;
		for(size_t i=0; i<sizeof(B); i++){
			uint8_t byte;
			read(byte);
			tmp |= (static_cast<B>(byte) << (i*8));
		}
		T result;
		std::memcpy(&result, &tmp, sizeof(T));
		return result;
	}

private:
	time::duration delta_time = time::immediate();

	template<class T>
	void read_number(T &value, typecode_e code){
		switch(code){
		case typecode_t::TYPE_NULL: value = 0; break;
		case typecode_t::TYPE_BOOL:
		case typecode_t::TYPE_U8: value = static_cast<T>(read_le<uint8_t>()); break;
		case typecode_t::TYPE_I8: value = static_cast<T>(read_le<int8_t>()); break;
		case typecode_t::TYPE_U16: value = static_cast<T>(read_le<uint16_t>()); break;
		case typecode_t::TYPE_I16: value = static_cast<T>(read_le<int16_t>()); break;
		case typecode_t::TYPE_U32: value = static_cast<T>(read_le<uint32_t>()); break;
		case typecode_t::TYPE_I32: value = static_cast<T>(read_le<int32_t>()); break;
		case typecode_t::TYPE_U64: value = static_cast<T>(read_le<uint64_t>()); break;
		case typecode_t::TYPE_I64:
		case typecode_t::TYPE_DURATION: value = static_cast<T>(read_le<int64_t>()); break;
		case typecode_t::TYPE_F32: value = static_cast<T>(read_le<float>()); break;
		case typecode_t::TYPE_F64: value = static_cast<T>(read_le<double>()); break;
		default:
			skip(code);
		}
	}

	template<class T>
	void read_set(T &value, typecode_e code){
		using K = typename T::key_type;
		if(code == typecode_t::TYPE_LIST){
			const auto size = read_le<uint64_t>();
			for(size_t i=0; i<size; i++){
				value.insert(read_type<K>());
			}
		}else if(code == typecode_t::TYPE_PAIR){
			value.insert(read_type<K>());
			value.insert(read_type<K>());
		}else if(code == typecode_t::TYPE_TUPLE){
			const auto size = read_le<uint64_t>();
			for(size_t i=0; i<size; i++){
				value.insert(read_type<K>());
			}
		}else{
			skip(code);
		}
	}

	template<class T>
	void read_map(T &value, typecode_e code){
		using K = typename T::key_type;
		using V = typename T::mapped_type;
		if(code == typecode_t::TYPE_MAP){
			const auto size = read_le<uint64_t>();
			for(size_t i=0; i<size; i++){
				const auto k = read_type<K>();
				const auto v = read_type<V>();
				value[k] = v;
			}
		}else if(code == typecode_t::TYPE_LIST){
			const auto size = read_le<uint64_t>();
			for(size_t i=0; i<size; i++){
				std::pair<K, V> item;
				read_type(item);
				value[item.first] = item.second;
			}
		}else{
			skip(code);
		}
	}

	// N = remaining elements
	template<size_t N, class... Ts>
	typename std::enable_if<(N>0)>::type read_tuple_elements(std::tuple<Ts...> &value, size_t size){
		constexpr size_t I = sizeof...(Ts) - N;
		if(I < size){
			read_type(std::get<I>(value));
			read_tuple_elements<N-1>(value, size);
		}
	}
	template<size_t N, class... Ts>
	typename std::enable_if<(N==0)>::type read_tuple_elements(std::tuple<Ts...> &value, size_t size){
	}
};


}
}
