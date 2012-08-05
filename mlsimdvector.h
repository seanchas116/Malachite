#ifndef MLSIMDVECTOR_H
#define MLSIMDVECTOR_H

#include <stdint.h>
#include <emmintrin.h>
#include <cstdlib>
#include <new>

#include "mlglobal.h"

#define ML_ALIGN_16BYTE \
public: \
	static void *operator new(size_t size) { \
		return mlAllocateAlignedMemory(size, 16); \
	} \
	static void operator delete(void *p) { \
		mlFreeAlignedMemory(p); \
	} \
	static void *operator new(size_t size, void *buf) { \
		Q_UNUSED(size); \
		return buf; \
	} \
	static void operator delete(void *p, void *buf) { \
		Q_UNUSED(p); \
		Q_UNUSED(buf); \
	} \
	static void *operator new[](size_t size) { \
		return mlAllocateAlignedMemory(size, 16); \
	} \
	static void operator delete[](void *p) { \
		mlFreeAlignedMemory(p); \
	} \
	static void *operator new[](size_t size, void *buf) { \
		Q_UNUSED(size); \
		return buf; \
	} \
	static void operator delete[](void *p, void *buf) { \
		Q_UNUSED(p); \
		Q_UNUSED(buf); \
	}

// SIMD wrapper classes
// These classes currently use SSE2 instructions. To make them use AVX or something, simply rewrite them.

class MLSimdF4;
class MLSimd32I4;

class MALACHITESHARED_EXPORT MLSimd32I4
{
public:
	
	MLSimd32I4() {}
	MLSimd32I4(int s) { e[0] = s; e[1] = s; e[2] = s; e[3] = s; }
	MLSimd32I4(int e0, int e1, int e2, int e3) { e[0] = e0; e[1] = e1; e[2] = e2; e[3] = e3; }
	MLSimd32I4(__v4si v) : v(v) {}
	
	int &operator[](int i) { return e[i]; }
	
	MLSimd32I4 &operator+=(const MLSimd32I4 &other) { v += other.v; return *this; }
	MLSimd32I4 &operator-=(const MLSimd32I4 &other) { v -= other.v; return *this; }
	MLSimd32I4 &operator*=(const MLSimd32I4 &other) { v *= other.v; return *this; }
	MLSimd32I4 &operator/=(const MLSimd32I4 &other) { v /= other.v; return *this; }
	
	MLSimd32I4 &operator+=(int value) { v += MLSimd32I4(value).v; return *this; }
	MLSimd32I4 &operator-=(int value) { v -= MLSimd32I4(value).v; return *this; }
	MLSimd32I4 &operator*=(int value) { v *= MLSimd32I4(value).v; return *this; }
	MLSimd32I4 &operator/=(int value) { v /= MLSimd32I4(value).v; return *this; }
	
	MLSimd32I4 operator+(const MLSimd32I4 &other) const { MLSimd32I4 r; r.v = v + other.v; return r; }
	MLSimd32I4 operator-(const MLSimd32I4 &other) const { MLSimd32I4 r; r.v = v - other.v; return r; }
	MLSimd32I4 operator*(const MLSimd32I4 &other) const { MLSimd32I4 r; r.v = v * other.v; return r; }
	MLSimd32I4 operator/(const MLSimd32I4 &other) const { MLSimd32I4 r; r.v = v / other.v; return r; }
	
	operator __v4si() { return v; }
	
	union
	{
		int e[4];
		__v4si v;
	};
};

inline MLSimd32I4 operator+(const MLSimd32I4 &x, int s) { return x + MLSimd32I4(s); }
inline MLSimd32I4 operator-(const MLSimd32I4 &x, int s) { return x - MLSimd32I4(s); }
inline MLSimd32I4 operator*(const MLSimd32I4 &x, int s) { return x * MLSimd32I4(s); }
inline MLSimd32I4 operator/(const MLSimd32I4 &x, int s) { return x / MLSimd32I4(s); }

inline MLSimd32I4 operator+(int s, const MLSimd32I4 &x) { return x + MLSimd32I4(s); }
inline MLSimd32I4 operator-(int s, const MLSimd32I4 &x) { return x - MLSimd32I4(s); }
inline MLSimd32I4 operator*(int s, const MLSimd32I4 &x) { return x * MLSimd32I4(s); }
inline MLSimd32I4 operator/(int s, const MLSimd32I4 &x) { return x / MLSimd32I4(s); }

class MALACHITESHARED_EXPORT MLSimdF4
{
public:
	
	MLSimdF4() {}
	MLSimdF4(float s) { e[0] = s; e[1] = s; e[2] = s; e[3] = s; }
	MLSimdF4(float e0, float e1, float e2, float e3) { e[0] = e0; e[1] = e1; e[2] = e2; e[3] = e3; }
	MLSimdF4(__v4sf v) : v(v) {}
	
	float &operator[](int i) { return e[i]; }
	const float &operator[](int i) const { return e[i]; }
	
	MLSimdF4 &operator+=(const MLSimdF4 &other) { v += other.v; return *this; }
	MLSimdF4 &operator-=(const MLSimdF4 &other) { v -= other.v; return *this; }
	MLSimdF4 &operator*=(const MLSimdF4 &other) { v *= other.v; return *this; }
	MLSimdF4 &operator/=(const MLSimdF4 &other) { v /= other.v; return *this; }
	
	MLSimdF4 &operator+=(float value) { v += MLSimdF4(value).v; return *this; }
	MLSimdF4 &operator-=(float value) { v -= MLSimdF4(value).v; return *this; }
	MLSimdF4 &operator*=(float value) { v *= MLSimdF4(value).v; return *this; }
	MLSimdF4 &operator/=(float value) { v /= MLSimdF4(value).v; return *this; }
	
	MLSimdF4 operator+(const MLSimdF4 &other) const { MLSimdF4 r; r.v = v + other.v; return r; }
	MLSimdF4 operator-(const MLSimdF4 &other) const { MLSimdF4 r; r.v = v - other.v; return r; }
	MLSimdF4 operator*(const MLSimdF4 &other) const { MLSimdF4 r; r.v = v * other.v; return r; }
	MLSimdF4 operator/(const MLSimdF4 &other) const { MLSimdF4 r; r.v = v / other.v; return r; }
	
	MLSimdF4 &operator=(const MLSimd32I4 &iv) { v = __builtin_ia32_cvtdq2ps(iv.v); return *this; }
	
	operator __v4sf() { return v; }
	
	union
	{
		float e[4];
		__v4sf v;
	};
};

inline MLSimdF4 operator+(const MLSimdF4 &x, float s) { return x + MLSimdF4(s); }
inline MLSimdF4 operator-(const MLSimdF4 &x, float s) { return x - MLSimdF4(s); }
inline MLSimdF4 operator*(const MLSimdF4 &x, float s) { return x * MLSimdF4(s); }
inline MLSimdF4 operator/(const MLSimdF4 &x, float s) { return x / MLSimdF4(s); }

inline MLSimdF4 operator+(float s, const MLSimdF4 &x) { return x + MLSimdF4(s); }
inline MLSimdF4 operator-(float s, const MLSimdF4 &x) { return x - MLSimdF4(s); }
inline MLSimdF4 operator*(float s, const MLSimdF4 &x) { return x * MLSimdF4(s); }
inline MLSimdF4 operator/(float s, const MLSimdF4 &x) { return x / MLSimdF4(s); }




inline MLSimd32I4 mlSimdRound(const MLSimdF4 &fv) { return MLSimd32I4(__builtin_ia32_cvtps2dq(fv.v)); }

inline MLSimdF4 mlSimdMax(const MLSimdF4 &v1, const MLSimdF4 &v2) { return MLSimdF4(__builtin_ia32_maxps(v1.v, v2.v)); }
inline MLSimdF4 mlSimdMin(const MLSimdF4 &v1, const MLSimdF4 &v2) { return MLSimdF4(__builtin_ia32_minps(v1.v, v2.v)); }

inline MLSimdF4 mlSimdBound(const MLSimdF4 &min, const MLSimdF4 &v, const MLSimdF4 &max)
{
	MLSimdF4 r;
	r = mlSimdMax(min, v);
	r = mlSimdMin(r, max);
	return r;
}

inline MLSimdF4 mlSimdBound(float min, const MLSimdF4 &v, float max) { return mlSimdBound(MLSimdF4(min), v, MLSimdF4(max)); }
inline MLSimd32I4 mlSimdEqual(const MLSimdF4 &v1, const MLSimdF4 &v2) { return MLSimd32I4(reinterpret_cast<__v4si>(__builtin_ia32_cmpeqps(v1.v, v2.v))); }
inline MLSimd32I4 mlSimdNotEqual(const MLSimdF4 &v1, const MLSimdF4 &v2) { return MLSimd32I4(reinterpret_cast<__v4si>(__builtin_ia32_cmpneqps(v1.v, v2.v))); }
inline MLSimd32I4 mlSimdLessThan(const MLSimdF4 &v1, const MLSimdF4 &v2) { return MLSimd32I4(reinterpret_cast<__v4si>(__builtin_ia32_cmpltps(v1.v, v2.v))); }
inline MLSimd32I4 mlSimdLessThanOrEqual(const MLSimdF4 &v1, const MLSimdF4 &v2) { return MLSimd32I4(reinterpret_cast<__v4si>(__builtin_ia32_cmpleps(v1.v, v2.v))); }
inline MLSimd32I4 mlSimdGreaterThan(const MLSimdF4 &v1, const MLSimdF4 &v2) { return MLSimd32I4(reinterpret_cast<__v4si>(__builtin_ia32_cmpltps(v2.v, v1.v))); }
inline MLSimd32I4 mlSimdGreaterThanOrEqual(const MLSimdF4 &v1, const MLSimdF4 &v2) { return MLSimd32I4(reinterpret_cast<__v4si>(__builtin_ia32_cmpleps(v2.v, v1.v))); }

inline MLSimdF4 mlSimdSqrt(const MLSimdF4 &v) { return MLSimdF4(__builtin_ia32_sqrtps(v.v)); }

#endif // MLSIMDVECTOR_H
