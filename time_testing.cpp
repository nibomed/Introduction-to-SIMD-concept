#include <iostream>
#include <immintrin.h>
#include <Windows.h>
#include <cmath>

LARGE_INTEGER ts1, ts2;

#define USE_SIMD

float randomFloat(float min, float max, float den) {
	int div = (max - min)*den;
	int r = rand() % div;
	return (r / den) + min;
}

int addArrays() {	
	const int size = 20000;
	__declspec(align(32)) __int32 a[size], b[size], c[size];
	__m128i *_a = (__m128i *) a; 
	__m128i *_b = (__m128i *) b; 
	__m128i *_c = (__m128i *) c; 

	for(int i = 0; i < size; i++) {
		a[i] = rand() % 1000000;
		b[i] = rand() % 1000000;
	}

	QueryPerformanceCounter(&ts1);
#ifdef USE_SIMD
	for(int i = 0; i < size / 4; i++) { //87
		 _c[i] = _mm_add_epi32(_a[i], _b[i]);
	}
#else
	for(int i = 0; i < size; i++) {  //338
		 c[i] = a[i] + b[i];
	}
#endif
	QueryPerformanceCounter(&ts2);

	static bool is_first = true;
	if (is_first)
		for(int i = 0; i< 5; i++)
			std::cout << a[i] << " + " << b[i] << " = " << c[i] << "\n";
	is_first = false;

	return ts2.QuadPart - ts1.QuadPart;
}

int calculateLength() {	
	const int size = 20000;
	__declspec(align(32)) float x[size], y[size], l[size];
	__declspec(align(32)) const float px = 20, py = 10;
	float const  randomrRange = 5, randomDensity = 1000;
	__m128 *_x = (__m128 *) x; 
	__m128 *_y = (__m128 *) y; 
	__m128 *_l = (__m128 *) l; 

	for(int i = 0; i < size; i++) {
		x[i] = randomFloat(px - randomrRange, px + randomrRange, randomDensity);
		y[i] = randomFloat(py - randomrRange, py + randomrRange, randomDensity);
	}

	QueryPerformanceCounter(&ts1);
#ifdef USE_SIMD
	__m128 _px = _mm_load_ps1(&px); 
	__m128 _py = _mm_load_ps1(&py);

	for(int i = 0; i < size / 4; i++) { //314
		 __m128 _dx = _mm_sub_ps(_x[i], _px);
		 __m128 _dy = _mm_sub_ps(_y[i], _py);
		 _l[i] = _mm_sqrt_ps(_mm_add_ps(_mm_mul_ps(_dx, _dx), _mm_mul_ps(_dy, _dy))); //260
	}
#else
	for(int i = 0; i < size; i++) {
		float dx = px - x[i];
		float dy = py - y[i];
		l[i] = std::sqrt(dx*dx+dy*dy); //default sqrt so slow
	}
#endif
	QueryPerformanceCounter(&ts2);

	static bool is_first = true;
	if (is_first)
		for(int i = 0; i< 5; i++)
			std::cout << l[i] << " " << x[i] << " : " << y[i] << "\n";
	is_first = false;

	return ts2.QuadPart - ts1.QuadPart;
}

int main() {	
	const int num = 1000;
	int sum = 0;
	auto f = &calculateLength;
	f();
	for (int i = 0; i < num; i++)
	{
		int current = f();
		sum += current;
	}

	std::cout << "average: " << sum / num << "\n";

	int j;
	std::cin >> j;
	return 0;
}