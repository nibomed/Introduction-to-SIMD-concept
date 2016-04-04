#include <iostream>
#include <immintrin.h>

#include <string>
#include <sstream>

template <typename T>
std::string __m128i_toString(const __m128i var) {
    std::stringstream sstr;
    const T* values = (const T*) &var;
    if (sizeof(T) == 1) {
        for (unsigned int i = 0; i < sizeof(__m128i); i++) {
            sstr << (int) values[i] << " ";
        }
    } else {
        for (unsigned int i = 0; i < sizeof(__m128i) / sizeof(T); i++) {
            sstr << values[i] << " ";
        }
    }
    return sstr.str();
}

int main () {
	__m128i a = _mm_set_epi32 (5, 9, 44, 1);
	__m128i b = _mm_set_epi32 (3, 19, 98, 101);
	__m128i c = _mm_add_epi32 (a, b);

	std::cout << __m128i_toString<__int32>(c) << std::endl;
	int i;
	std::cin>> i;
	return 0;
}	