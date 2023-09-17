#pragma once


#if __cplusplus == 201103L

#include <memory>

namespace std{

// Introduced in C++14
template<class T, class... Args>
unique_ptr<T> make_unique(Args&&... args){
	return unique_ptr<T>(new T(forward<Args>(args)...));
}

}

#endif

