#pragma once

#include <type_traits>
#include <cstddef>
#include <cstdint>


namespace asgard{
namespace util{


template<size_t width>
using uint_w = typename std::enable_if<
	width >= 1 && width <= 64,
	typename std::conditional<
		width <= 8,
		uint8_t,
		typename std::conditional<
			width <= 16,
			uint16_t,
			typename std::conditional<
				width <= 32,
				uint32_t,
				uint64_t
			>::type
		>::type
	>::type
>::type;


template<class T, size_t width=sizeof(T)*8>
T reflect(const T &value){
	static_assert(width <= sizeof(T)*8, "Cannot reflect more bits than the type has");
	T result = 0;
	const T one = 1;
	for(size_t i=0; i<width; i++){
		result |= (((value & (one << i)) >> i) << (width - i - 1));
	}
	return result;
}


template<size_t width, uint_w<width> poly, bool ref_in, bool ref_out, uint_w<width> init, uint_w<width> xorout>
class CRC{
public:
	using R = uint_w<width>;
	static const size_t bit_size = width;
	static const R polygon = poly;
	static const R initialization = init;
	static const R final_xor = xorout;
	static const bool reflects_input = ref_in;
	static const bool reflects_output = ref_out;

	CRC(){
		reset();
	}

	void reset(){
		if(ref_in){
			current = reflect<decltype(init), width>(init);
		}else{
			current = init;
		}
	}

	void update(const char *data, size_t length){
		for(size_t i=0; i<length; i++){
			const R c = static_cast<R>(data[i]);
			if(ref_in){
				current = table[(current ^ c) & 0xFF] ^ (current >> 8);
			}else if(width < 8){
				current = table[((current << (8 - width)) ^ c) & 0xFF] ^ (current << 8);
			}else{
				current = table[((current >> (width - 8)) ^ c) & 0xFF] ^ (current << 8);
			}
		}
	}

	R value() const{
		R result = current;
		if(ref_in != ref_out){
			result = reflect<decltype(result), width>(result);
		}
		return (result ^ xorout) & mask;
	}

	// shortcut
	static R compute(const char *data, size_t length){
		CRC crc;
		crc.update(data, length);
		return crc.value();
	}


	// direct calculation. Only use to generate a table or if you don't care about performance.
	static R calc(const char *data, size_t length){
		R result = init;
		for(size_t i=0; i<length; i++){
			if(width < 8){
				unsigned char c = data[i];
				if(ref_in){
					c = reflect(c);
				}
				for(unsigned char j=0x80; j!=0; j>>=1){
					R bit;
					if((c & j) != 0){
						bit = ~result & high_bit;
					}else{
						bit = result & high_bit;
					}
					result <<= 1;
					if(bit != 0){
						result = result ^ poly;
					}
				}
			}else{
				// slightly faster but only works for width >= 8
				R c;
				{
					unsigned char cc = data[i];
					if(ref_in){
						cc = reflect(cc);
					}
					c = static_cast<R>(cc) << (width - 8);
				}
				for(size_t j=0; j<8; j++){
					if((result ^ c) & high_bit){
						result = (result << 1) ^ poly;
					}else{
						result <<= 1;
					}
					c <<= 1;
				}
			}
		}

		if(ref_out){
			result = reflect<decltype(result), width>(result);
		}
		return (result ^ xorout) & mask;
	}

	static void calc_table(R (&result)[256]){
		for(size_t i=0; i<256; i++){
			const char c = i;
			// ref_out = ref_in, init = xorout = 0
			result[i] = CRC<width, poly, ref_in, ref_in, 0, 0>::calc(&c, 1);
		}
	}

private:
	static const R high_bit = static_cast<R>(1) << (width - 1);
	static const R mask = static_cast<R>(-1) >> (sizeof(R)*8 - width);
	static const R table[256];
	R current;
};


// Definition, so that the class can be used without a table.
// Can be overriden by template specialization.
template<size_t width, uint_w<width> poly, bool ref_in, bool ref_out, uint_w<width> init, uint_w<width> xorout>
const uint_w<width> CRC<width, poly, ref_in, ref_out, init, xorout>::table[256] = {0};


}
}
