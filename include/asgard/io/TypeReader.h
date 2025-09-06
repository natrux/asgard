#pragma once

#include <asgard/io/BufferedInput.h>
#include <asgard/code/Typecode.h>
#include <asgard/time/time.h>
#include <asgard/data/Value.h>
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
namespace io{


class TypeReader : public BufferedInput{
public:
	TypeReader(std::shared_ptr<InputSource> source);

	time::time get_remote_epoch() const;
	void set_remote_epoch(const time::time &time);
	void set_remote_since_epoch(const time::duration &since);

	code::Typecode read_typecode();

	template<class T>
	T read_type(){
		T value;
		read_type(value);
		return value;
	}

	template<class T>
	void read_type(T &value){
		const auto type = read_typecode();
		read_type(value, type);
	}

	template<class T>
	T read_type(const code::Typecode &type){
		T value;
		read_type(value, type);
		return value;
	}

	void read_type(bool &value, const code::Typecode &type);
	void read_type(uint8_t &value, const code::Typecode &type);
	void read_type(int8_t &value, const code::Typecode &type);
	void read_type(uint16_t &value, const code::Typecode &type);
	void read_type(int16_t &value, const code::Typecode &type);
	void read_type(uint32_t &value, const code::Typecode &type);
	void read_type(int32_t &value, const code::Typecode &type);
	void read_type(uint64_t &value, const code::Typecode &type);
	void read_type(int64_t &value, const code::Typecode &type);
	void read_type(float &value, const code::Typecode &type);
	void read_type(double &value, const code::Typecode &type);
	void read_type(std::string &value, const code::Typecode &type);
	void read_type(time::time &value, const code::Typecode &type);
	void read_type(time::wall_time &value, const code::Typecode &type);
	void read_type(time::duration &value, const code::Typecode &type);
	void read_type(data::Value &value, const code::Typecode &type);
	void read_type(data::Enum &value, const code::Typecode &type);

	template<class T>
	void read_type(std::vector<T> &value, const code::Typecode &type){
		if(type.code == code::Typecode::TYPE_LIST){
			const auto size = read_le<code::length_t>();
			const auto &sub_type = type.sub_types.at(0);
			for(size_t i=0; i<size; i++){
				value.push_back(read_type<T>(sub_type));
			}
		}else if(type.code == code::Typecode::TYPE_PAIR){
			value.push_back(read_type<T>(type.sub_types.at(0)));
			value.push_back(read_type<T>(type.sub_types.at(1)));
		}else if(type.code == code::Typecode::TYPE_TUPLE){
			const auto size = type.sub_types.size();
			for(size_t i=0; i<size; i++){
				value.push_back(read_type<T>(type.sub_types.at(i)));
			}
		}else{
			skip(type);
		}
	}

	template<class T, size_t N>
	void read_type(std::array<T, N> &value, const code::Typecode &type){
		if(type.code == code::Typecode::TYPE_LIST){
			const auto &sub_type = type.sub_types.at(0);
			const auto size = read_le<code::length_t>();
			for(size_t i=0; i<size; i++){
				if(i < N){
					value[i] = read_type<T>(sub_type);
				}else{
					skip(sub_type);
				}
			}
		}else if(type.code == code::Typecode::TYPE_PAIR){
			const auto &sub_type_1 = type.sub_types.at(0);
			const auto &sub_type_2 = type.sub_types.at(1);
			if(N >= 2){
				value[0] = read_type<T>(sub_type_1);
				value[1] = read_type<T>(sub_type_2);
			}else if(N >= 1){
				value[0] = read_type<T>(sub_type_1);
				skip(sub_type_2);
			}else{
				skip(sub_type_1);
				skip(sub_type_2);
			}
		}else if(type.code == code::Typecode::TYPE_TUPLE){
			const auto size = type.sub_types.size();
			for(size_t i=0; i<size; i++){
				const auto &sub_type = type.sub_types.at(i);
				if(i < N){
					value[i] = read_type<T>(sub_type);
				}else{
					skip(sub_type);
				}
			}
		}else{
			skip(type);
		}
	}

	template<class T>
	void read_type(std::set<T> &value, const code::Typecode &type){
		read_set(value, type);
	}

	template<class T>
	void read_type(std::unordered_set<T> &value, const code::Typecode &type){
		read_set(value, type);
	}

	template<class K, class V>
	void read_type(std::map<K, V> &value, const code::Typecode &type){
		read_map(value, type);
	}

	template<class K, class V>
	void read_type(std::unordered_map<K, V> &value, const code::Typecode &type){
		read_map(value, type);
	}

	template<class T, class U>
	void read_type(std::pair<T, U> &value, const code::Typecode &type){
		if(type.code == code::Typecode::TYPE_PAIR){
			value.first = read_type<T>(type.sub_types.at(0));
			value.second = read_type<U>(type.sub_types.at(1));
		}else if(type.code == code::Typecode::TYPE_LIST){
			const auto &sub_type = type.sub_types.at(0);
			const auto size = read_le<code::length_t>();
			if(size >= 1){
				value.first = read_type<T>(sub_type);
			}
			if(size >= 2){
				value.second = read_type<U>(sub_type);
			}
			for(size_t i=2; i<size; i++){
				skip(sub_type);
			}
		}else{
			skip(type);
		}
	}

	template<class... Ts>
	void read_type(std::tuple<Ts...> &value, const code::Typecode &type){
		constexpr size_t N = sizeof...(Ts);
		if(type.code == code::Typecode::TYPE_TUPLE){
			const auto size = type.sub_types.size();
			read_tuple_elements<N>(value, size, type.sub_types);
			for(size_t i=N; i<size; i++){
				skip(type.sub_types.at(i));
			}
		}else if(type.code == code::Typecode::TYPE_PAIR){
			read_tuple_elements<N>(value, 2, type.sub_types);
			for(size_t i=N; i<2; i++){
				skip(type.sub_types.at(i));
			}
		}else if(type.code == code::Typecode::TYPE_LIST){
			const auto &sub_type = type.sub_types.at(0);
			const auto size = read_le<code::length_t>();
			std::vector<code::Typecode> sub_typecodes;
			for(size_t i=0; i<size; i++){
				sub_typecodes.push_back(sub_type);
			}
			read_tuple_elements<N>(value, size, sub_typecodes);
			for(size_t i=N; i<size; i++){
				skip(sub_type);
			}
		}else{
			skip(type);
		}
	}

	template<class T>
	void read_type(std::optional<T> &value, const code::Typecode &type){
		if(type.code == code::Typecode::TYPE_OPTIONAL || type.code == code::Typecode::TYPE_POINTER){
			const bool flag = read_le<uint8_t>();
			if(flag){
				value = read_type<T>(type.sub_types.at(0));
			}
		//}else if(type matches get_typecode<T>()){
		}else{
			skip(type);
		}
	}

	template<class T>
	void read_type(std::shared_ptr<T> &value, const code::Typecode &type){
		if(type.code == code::Typecode::TYPE_OPTIONAL || type.code == code::Typecode::TYPE_POINTER){
			const bool flag = read_le<uint8_t>();
			if(flag){
				auto ptr = std::make_shared<typename std::remove_const<T>::type>();
				read_type(*ptr, type.sub_types.at(0));
				value = ptr;
			}
		//}else if(type matches get_typecode<T>()){
		}else{
			skip(type);
		}
	}

	void skip();
	void skip(const code::Typecode &type);

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

	void read_string(std::string &value);

private:
	time::time remote_epoch;

	template<class T>
	void read_number(T &value, code::typecode_e code){
		switch(code){
		case code::Typecode::TYPE_NULL: value = 0; break;
		case code::Typecode::TYPE_BOOL:
		case code::Typecode::TYPE_U8: value = static_cast<T>(read_le<uint8_t>()); break;
		case code::Typecode::TYPE_I8: value = static_cast<T>(read_le<int8_t>()); break;
		case code::Typecode::TYPE_U16: value = static_cast<T>(read_le<uint16_t>()); break;
		case code::Typecode::TYPE_I16: value = static_cast<T>(read_le<int16_t>()); break;
		case code::Typecode::TYPE_U32: value = static_cast<T>(read_le<uint32_t>()); break;
		case code::Typecode::TYPE_I32: value = static_cast<T>(read_le<int32_t>()); break;
		case code::Typecode::TYPE_U64: value = static_cast<T>(read_le<uint64_t>()); break;
		case code::Typecode::TYPE_I64:
		case code::Typecode::TYPE_DURATION: value = static_cast<T>(read_le<int64_t>()); break;
		case code::Typecode::TYPE_F32: value = static_cast<T>(read_le<float>()); break;
		case code::Typecode::TYPE_F64: value = static_cast<T>(read_le<double>()); break;
		default:
			skip(code);
		}
	}

	template<class T>
	void read_set(T &value, const code::Typecode &type){
		using K = typename T::key_type;
		if(type.code == code::Typecode::TYPE_LIST){
			const auto &sub_type = type.sub_types.at(0);
			const auto size = read_le<code::length_t>();
			for(size_t i=0; i<size; i++){
				value.insert(read_type<K>(sub_type));
			}
		}else if(type.code == code::Typecode::TYPE_PAIR){
			value.insert(read_type<K>(type.sub_types.at(0)));
			value.insert(read_type<K>(type.sub_types.at(1)));
		}else if(type.code == code::Typecode::TYPE_TUPLE){
			const auto size = type.sub_types.size();
			for(size_t i=0; i<size; i++){
				value.insert(read_type<K>(type.sub_types.at(i)));
			}
		}else{
			skip(type);
		}
	}

	template<class T>
	void read_map(T &value, const code::Typecode &type){
		using K = typename T::key_type;
		using V = typename T::mapped_type;
		if(type.code == code::Typecode::TYPE_MAP){
			const auto &key_type = type.sub_types.at(0);
			const auto &value_type = type.sub_types.at(1);
			const auto size = read_le<code::length_t>();
			for(size_t i=0; i<size; i++){
				const auto k = read_type<K>(key_type);
				const auto v = read_type<V>(value_type);
				value[k] = v;
			}
		}else if(type.code == code::Typecode::TYPE_LIST){
			const auto &sub_type = type.sub_types.at(0);
			const auto size = read_le<code::length_t>();
			for(size_t i=0; i<size; i++){
				std::pair<K, V> item;
				read_type(item, sub_type);
				value[item.first] = item.second;
			}
		}else{
			skip(type);
		}
	}

	// N = remaining elements
	template<size_t N, class... Ts>
	typename std::enable_if<(N>0)>::type read_tuple_elements(std::tuple<Ts...> &value, size_t size, const std::vector<code::Typecode> &sub_types){
		constexpr size_t I = sizeof...(Ts) - N;
		if(I < size){
			read_type(std::get<I>(value), sub_types.at(I));
			read_tuple_elements<N-1>(value, size, sub_types);
		}
	}
	template<size_t N, class... Ts>
	typename std::enable_if<(N==0)>::type read_tuple_elements(std::tuple<Ts...> &value, size_t size, const std::vector<code::Typecode> &sub_types){
	}
};


}
}
