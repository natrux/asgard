#pragma once

#include <asgard/code/Typecode.h>
#include <asgard/time/time.h>

#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <array>
#include <set>
#include <unordered_set>
#include <optional>
#include <memory>


namespace asgard{
namespace code{


template<class T>
struct get_typecode_t{
	static Typecode value();
};

template<class T>
Typecode get_typecode(){
	return get_typecode_t<T>::value();
}





template<>
struct get_typecode_t<std::nullptr_t>{
	static Typecode value(){
		static Typecode instance(Typecode::TYPE_NULL);
		return instance;
	}
};

template<>
struct get_typecode_t<bool>{
	static Typecode value(){
		static Typecode instance(Typecode::TYPE_BOOL);
		return instance;
	}
};

template<>
struct get_typecode_t<uint8_t>{
	static Typecode value(){
		static Typecode instance(Typecode::TYPE_U8);
		return instance;
	}
};

template<>
struct get_typecode_t<int8_t>{
	static Typecode value(){
		static Typecode instance(Typecode::TYPE_I8);
		return instance;
	}
};

template<>
struct get_typecode_t<uint16_t>{
	static Typecode value(){
		static Typecode instance(Typecode::TYPE_U16);
		return instance;
	}
};

template<>
struct get_typecode_t<int16_t>{
	static Typecode value(){
		static Typecode instance(Typecode::TYPE_I16);
		return instance;
	}
};

template<>
struct get_typecode_t<uint32_t>{
	static Typecode value(){
		static Typecode instance(Typecode::TYPE_U32);
		return instance;
	}
};

template<>
struct get_typecode_t<int32_t>{
	static Typecode value(){
		static Typecode instance(Typecode::TYPE_I32);
		return instance;
	}
};

template<>
struct get_typecode_t<uint64_t>{
	static Typecode value(){
		static Typecode instance(Typecode::TYPE_U64);
		return instance;
	}
};

template<>
struct get_typecode_t<int64_t>{
	static Typecode value(){
		static Typecode instance(Typecode::TYPE_I64);
		return instance;
	}
};

template<>
struct get_typecode_t<float>{
	static Typecode value(){
		static Typecode instance(Typecode::TYPE_F32);
		return instance;
	}
};

template<>
struct get_typecode_t<double>{
	static Typecode value(){
		static Typecode instance(Typecode::TYPE_F64);
		return instance;
	}
};

template<>
struct get_typecode_t<time::duration>{
	static Typecode value(){
		static Typecode instance(Typecode::TYPE_DURATION);
		return instance;
	}
};

template<>
struct get_typecode_t<time::time>{
	static Typecode value(){
		static Typecode instance(Typecode::TYPE_DURATION);
		return instance;
	}
};

template<>
struct get_typecode_t<time::wall_time>{
	static Typecode value(){
		static Typecode instance(Typecode::TYPE_DURATION);
		return instance;
	}
};

template<>
struct get_typecode_t<std::string>{
	static Typecode value(){
		static Typecode instance(Typecode::TYPE_STRING);
		return instance;
	}
};

template<class T>
struct get_typecode_t<std::vector<T>>{
	static Typecode value(){
		static Typecode instance(Typecode::TYPE_LIST, {get_typecode<T>()});
		return instance;
	}
};

template<class T, size_t N>
struct get_typecode_t<std::array<T, N>>{
	static Typecode value(){
		static Typecode instance(Typecode::TYPE_LIST, {get_typecode<T>()});
		return instance;
	}
};

template<class T>
struct get_typecode_t<std::set<T>>{
	static Typecode value(){
		static Typecode instance(Typecode::TYPE_LIST, {get_typecode<T>()});
		return instance;
	}
};

template<class T>
struct get_typecode_t<std::unordered_set<T>>{
	static Typecode value(){
		static Typecode instance(Typecode::TYPE_LIST, {get_typecode<T>()});
		return instance;
	}
};

template<class K, class V>
struct get_typecode_t<std::map<K, V>>{
	static Typecode value(){
		static Typecode instance(Typecode::TYPE_MAP, {get_typecode<K>(), get_typecode<V>()});
		return instance;
	}
};

template<class K, class V>
struct get_typecode_t<std::unordered_map<K, V>>{
	static Typecode value(){
		static Typecode instance(Typecode::TYPE_MAP, {get_typecode<K>(), get_typecode<V>()});
		return instance;
	}
};

template<class U, class V>
struct get_typecode_t<std::pair<U, V>>{
	static Typecode value(){
		static Typecode instance(Typecode::TYPE_PAIR, {get_typecode<U>(), get_typecode<V>()});
		return instance;
	}
};

template<class... Ts>
struct get_typecode_t<std::tuple<Ts...>>{
	template<class... Us>
	static typename std::enable_if<(sizeof...(Us)==0), std::vector<Typecode>>::type typecode_list(){
		return {};
	}

	template<class F, class... Us>
	static std::vector<Typecode> typecode_list(){
		std::vector<Typecode> result = typecode_list<Us...>();
		result.insert(result.begin(), get_typecode<F>());
		return result;
	}

	static Typecode value(){
		static Typecode instance(Typecode::TYPE_TUPLE, typecode_list<Ts...>());
		return instance;
	}
};

template<class T>
struct get_typecode_t<std::optional<T>>{
	static Typecode value(){
		static Typecode instance(Typecode::TYPE_OPTIONAL, {get_typecode<T>()});
		return instance;
	}
};

template<class T>
struct get_typecode_t<std::shared_ptr<T>>{
	static Typecode value(){
		static Typecode instance(Typecode::TYPE_POINTER, {get_typecode<typename std::remove_const<T>::type>()});
		return instance;
	}
};


}
}
