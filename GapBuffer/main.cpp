#include "./iterators/const_iterator.h"
#include "./iterators/iterator.h"
#include "GapBuffer.h"
#include <iostream>
#include <algorithm>
#include <type_traits>

using namespace std;

//По умолчанию типы false
//template <typename T>
//struct is_gap : std::false_type { };
//
//Перегружаем шаблон так, что для GapBuffer он теперь true.
//template <>
//struct is_gap<GapBuffer> : std::true_type {};
//
//template <typename T, class = enable_if_t<is_gap<T>::value, T> >
//void make(const T& t) { return; }


int main() {
	GapBuffer gp;
	
	return 0;
}