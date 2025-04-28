#pragma once

#include <asgard/io/BufferedInput.h>
#include <asgard/io/type_code_t.h>
#include <asgard/time/time.h>

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
		const auto code = read_le<typecode_t>();
		read_type(value, code);
	}

	template<class T>
	void read_type(std::vector<T> &value, typecode_t code){
		if(code == TYPE_LIST){
			const auto size = read_le<uint64_t>();
			for(size_t i=0; i<size; i++){
				value.push_back(read_type<T>());
			}
		}else if(code == TYPE_PAIR){
			value.push_back(read_type<T>());
			value.push_back(read_type<T>());
		}else{
			skip(code);
		}
	}

	template<class T, size_t N>
	void read_type(std::array<T, N> &value, typecode_t code){
		if(code == TYPE_LIST){
			const auto size = read_le<uint64_t>();
			for(size_t i=0; i<size; i++){
				if(i < N){
					value[i] = read_type<T>();
				}else{
					skip();
				}
			}
		}else if(code == TYPE_PAIR){
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
		}else{
			skip(code);
		}
	}

	template<class T>
	void read_type(std::set<T> &value, typecode_t code){
		if(code == TYPE_LIST){
			const auto size = read_le<uint64_t>();
			for(size_t i=0; i<size; i++){
				value.insert(read_type<T>());
			}
		}else if(code == TYPE_PAIR){
			value.insert(read_type<T>());
			value.insert(read_type<T>());
		}else{
			skip(code);
		}
	}

	template<class T>
	void read_type(std::unordered_set<T> &value, typecode_t code){
		if(code == TYPE_LIST){
			const auto size = read_le<uint64_t>();
			for(size_t i=0; i<size; i++){
				value.insert(read_type<T>());
			}
		}else if(code == TYPE_PAIR){
			value.insert(read_type<T>());
			value.insert(read_type<T>());
		}else{
			skip(code);
		}
	}

	template<class K, class V>
	void read_type(std::map<K, V> &value, typecode_t code){
		read_map(value, code);
	}

	template<class K, class V>
	void read_type(std::unordered_map<K, V> &value, typecode_t code){
		read_map(value, code);
	}

	template<class T, class U>
	void read_type(std::pair<T, U> &value, typecode_t code){
		if(code == TYPE_PAIR){
			value.first = read_type<T>();
			value.second = read_type<U>();
		}else if(code == TYPE_LIST){
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

	template<class T>
	void read_type(std::optional<T> &value, typecode_t code){
		if(code == TYPE_NULL){
			return;
		}
		T optional;
		read_type(optional, code);
		value = optional;
	}

	template<class T>
	void read_type(std::shared_ptr<T> &value, typecode_t code){
		if(code == TYPE_NULL){
			return;
		}
		auto ptr = std::make_shared<T>();
		read_type(*ptr, code);
		value = ptr;
	}

	void read_type(bool &value, typecode_t code);
	void read_type(int8_t &value, typecode_t code);
	void read_type(uint64_t &value, typecode_t code);
	void read_type(float &value, typecode_t code);
	void read_type(double &value, typecode_t code);
	void read_type(std::string &value, typecode_t code);
	void read_type(time::time &value, typecode_t code);
	void read_type(time::wall_time &value, typecode_t code);
	void read_type(time::duration &value, typecode_t code);

	template<class T>
	void read_type(T &value, typecode_t code){
		value.read_from(this, code);
	}

	void skip();
	void skip(typecode_t code);

private:
	time::duration delta_time;

	template<class T, class B=T>
	T read_le(){
		static_assert(sizeof(T) == sizeof(B));
		uint8_t bytes[sizeof(B)];
		read(bytes, sizeof(B));
		B tmp = 0;
		for(size_t i=0; i<sizeof(B); i++){
			tmp |= (static_cast<B>(bytes[i]) << (i*8));
		}
		T result;
		std::memcpy(&result, &tmp, sizeof(T));
		return result;
	}

	template<class T>
	void read_number(T &value, typecode_t code){
		switch(code){
		case TYPE_NULL: value = 0; break;
		case TYPE_BOOL:
		case TYPE_U8: value = static_cast<T>(read_le<uint8_t>()); break;
		case TYPE_I8: value = static_cast<T>(read_le<int8_t, uint8_t>()); break;
		case TYPE_U16: value = static_cast<T>(read_le<uint16_t>()); break;
		case TYPE_I16: value = static_cast<T>(read_le<int16_t, uint16_t>()); break;
		case TYPE_U32: value = static_cast<T>(read_le<uint32_t>()); break;
		case TYPE_I32: value = static_cast<T>(read_le<int32_t, uint32_t>()); break;
		case TYPE_U64: value = static_cast<T>(read_le<uint64_t>()); break;
		case TYPE_I64:
		case TYPE_DURATION: value = static_cast<T>(read_le<int64_t, uint64_t>()); break;
		case TYPE_F32: value = static_cast<T>(read_le<float, uint32_t>()); break;
		case TYPE_F64: value = static_cast<T>(read_le<double, uint64_t>()); break;
		default:
			skip(code);
		}
	}

	template<class T>
	void read_map(T &value, typecode_t code){
		if(code == TYPE_MAP){
			const auto size = read_le<uint64_t>();
			for(size_t i=0; i<size; i++){
				value[read_type<typename T::key_type>()] = read_type<typename T::mapped_type>();
			}
		}else if(code == TYPE_LIST){
			const auto size = read_le<uint64_t>();
			for(size_t i=0; i<size; i++){
				std::pair<typename T::key_type, typename T::mapped_type> item;
				read_type(item);
				value[item.first] = item.second;
			}
		}else{
			skip(code);
		}
	}
};


}
}
