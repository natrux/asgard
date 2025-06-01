#pragma once

#include <asgard/io/typecode_t.h>
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
namespace io{


template<class T>
struct get_typecode_t{
	static typecode_t value();
};

template<class T>
typecode_t get_typecode(){
	return get_typecode_t<T>::value();
}





template<>
struct get_typecode_t<std::nullptr_t>{
	static typecode_t value(){
		static typecode_t instance(typecode_t::TYPE_NULL);
		return instance;
	}
};

template<>
struct get_typecode_t<bool>{
	static typecode_t value(){
		static typecode_t instance(typecode_t::TYPE_BOOL);
		return instance;
	}
};

template<>
struct get_typecode_t<uint8_t>{
	static typecode_t value(){
		static typecode_t instance(typecode_t::TYPE_U8);
		return instance;
	}
};

template<>
struct get_typecode_t<int8_t>{
	static typecode_t value(){
		static typecode_t instance(typecode_t::TYPE_I8);
		return instance;
	}
};

template<>
struct get_typecode_t<uint16_t>{
	static typecode_t value(){
		static typecode_t instance(typecode_t::TYPE_U16);
		return instance;
	}
};

template<>
struct get_typecode_t<int16_t>{
	static typecode_t value(){
		static typecode_t instance(typecode_t::TYPE_I16);
		return instance;
	}
};

template<>
struct get_typecode_t<uint32_t>{
	static typecode_t value(){
		static typecode_t instance(typecode_t::TYPE_U32);
		return instance;
	}
};

template<>
struct get_typecode_t<int32_t>{
	static typecode_t value(){
		static typecode_t instance(typecode_t::TYPE_I32);
		return instance;
	}
};

template<>
struct get_typecode_t<uint64_t>{
	static typecode_t value(){
		static typecode_t instance(typecode_t::TYPE_U64);
		return instance;
	}
};

template<>
struct get_typecode_t<int64_t>{
	static typecode_t value(){
		static typecode_t instance(typecode_t::TYPE_I64);
		return instance;
	}
};

template<>
struct get_typecode_t<float>{
	static typecode_t value(){
		static typecode_t instance(typecode_t::TYPE_F32);
		return instance;
	}
};

template<>
struct get_typecode_t<double>{
	static typecode_t value(){
		static typecode_t instance(typecode_t::TYPE_F64);
		return instance;
	}
};

template<>
struct get_typecode_t<time::duration>{
	static typecode_t value(){
		static typecode_t instance(typecode_t::TYPE_DURATION);
		return instance;
	}
};

template<>
struct get_typecode_t<time::time>{
	static typecode_t value(){
		static typecode_t instance(typecode_t::TYPE_DURATION);
		return instance;
	}
};

template<>
struct get_typecode_t<time::wall_time>{
	static typecode_t value(){
		static typecode_t instance(typecode_t::TYPE_DURATION);
		return instance;
	}
};

template<>
struct get_typecode_t<std::string>{
	static typecode_t value(){
		static typecode_t instance(typecode_t::TYPE_STRING);
		return instance;
	}
};

template<class T>
struct get_typecode_t<std::vector<T>>{
	static typecode_t value(){
		static typecode_t instance(typecode_t::TYPE_LIST, {get_typecode<T>()});
		return instance;
	}
};

template<class T, size_t N>
struct get_typecode_t<std::array<T, N>>{
	static typecode_t value(){
		static typecode_t instance(typecode_t::TYPE_LIST, {get_typecode<T>()});
		return instance;
	}
};

template<class T>
struct get_typecode_t<std::set<T>>{
	static typecode_t value(){
		static typecode_t instance(typecode_t::TYPE_LIST, {get_typecode<T>()});
		return instance;
	}
};

template<class T>
struct get_typecode_t<std::unordered_set<T>>{
	static typecode_t value(){
		static typecode_t instance(typecode_t::TYPE_LIST, {get_typecode<T>()});
		return instance;
	}
};

template<class K, class V>
struct get_typecode_t<std::map<K, V>>{
	static typecode_t value(){
		static typecode_t instance(typecode_t::TYPE_MAP, {get_typecode<K>(), get_typecode<V>()});
		return instance;
	}
};

template<class K, class V>
struct get_typecode_t<std::unordered_map<K, V>>{
	static typecode_t value(){
		static typecode_t instance(typecode_t::TYPE_MAP, {get_typecode<K>(), get_typecode<V>()});
		return instance;
	}
};

template<class U, class V>
struct get_typecode_t<std::pair<U, V>>{
	static typecode_t value(){
		static typecode_t instance(typecode_t::TYPE_PAIR, {get_typecode<U>(), get_typecode<V>()});
		return instance;
	}
};

template<class... Ts>
struct get_typecode_t<std::tuple<Ts...>>{
	template<class... Us>
	static typename std::enable_if<(sizeof...(Us)==0), std::vector<typecode_t>>::type typecode_list(){
		return {};
	}

	template<class F, class... Us>
	static std::vector<typecode_t> typecode_list(){
		std::vector<typecode_t> result = typecode_list<Us...>();
		result.insert(result.begin(), get_typecode<F>());
		return result;
	};

	static typecode_t value(){
		static typecode_t instance(typecode_t::TYPE_TUPLE, typecode_list<Ts...>());
		return instance;
	};
};

template<class T>
struct get_typecode_t<std::optional<T>>{
	static typecode_t value(){
		static typecode_t instance(typecode_t::TYPE_OPTIONAL, {get_typecode<T>()});
		return instance;
	}
};

template<class T>
struct get_typecode_t<std::shared_ptr<T>>{
	static typecode_t value(){
		static typecode_t instance(typecode_t::TYPE_POINTER, {get_typecode<typename std::remove_const<T>::type>()});
		return instance;
	}
};


}
}
