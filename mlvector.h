#ifndef MLVECTOR_H
#define MLVECTOR_H

#include <stdint.h>
#include <emmintrin.h>
#include <cstdlib>
#include <cmath>
#include <new>
#include <QPointF>
#include <QMetaType>
#include <QTransform>

#include "mlglobal.h"
#include "mlmemory.h"

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

struct MLVec2I64
{
	ML_ALIGN_16BYTE
	
	MLVec2I64() {}
	MLVec2I64(int64_t s)
	{
		e[0] = s;
		e[1] = s;
	}
	MLVec2I64(int64_t e0, int64_t e1)
	{
		e[0] = e0;
		e[1] = e1;
	}
	MLVec2I64(__v2di v) : v(v) {}
	
	int64_t &operator[](int i) { return e[i]; }
	const int64_t &operator[](int i) const { return e[i]; }
	operator __v2di() { return v; }
	operator const __v2di() const { return v; }
	
	/*
	MLVec2I64 &operator+=(const MLVec2I64 &other) { v += other.v; return *this; }
	MLVec2I64 &operator-=(const MLVec2I64 &other) { v -= other.v; return *this; }
	MLVec2I64 &operator*=(const MLVec2I64 &other) { v *= other.v; return *this; }
	MLVec2I64 &operator/=(const MLVec2I64 &other) { v /= other.v; return *this; }
	
	MLVec2I64 &operator+=(int64_t value) { v += MLVec2I64(value).v; return *this; }
	MLVec2I64 &operator-=(int64_t value) { v -= MLVec2I64(value).v; return *this; }
	MLVec2I64 &operator*=(int64_t value) { v *= MLVec2I64(value).v; return *this; }
	MLVec2I64 &operator/=(int64_t value) { v /= MLVec2I64(value).v; return *this; }
	
	const MLVec2I64 operator+(const MLVec2I64 &other) const { MLVec2I64 r; r.v = v + other.v; return r; }
	const MLVec2I64 operator-(const MLVec2I64 &other) const { MLVec2I64 r; r.v = v - other.v; return r; }
	const MLVec2I64 operator*(const MLVec2I64 &other) const { MLVec2I64 r; r.v = v * other.v; return r; }
	const MLVec2I64 operator/(const MLVec2I64 &other) const { MLVec2I64 r; r.v = v / other.v; return r; }
	*/
	
	union
	{
		struct
		{
			
			int64_t x;
			int64_t y;
		};
		int64_t e[2];
		__v2di v;
	};
};

struct MLVec4I32
{
	ML_ALIGN_16BYTE
	
	MLVec4I32() {}
	MLVec4I32(int32_t s)
	{
		e[0] = s;
		e[1] = s;
		e[2] = s;
		e[3] = s;
	}
	MLVec4I32(int32_t e0, int32_t e1, int32_t e2, int32_t e3)
	{
		e[0] = e0;
		e[1] = e1;
		e[2] = e2;
		e[3] = e3;
	}
	MLVec4I32(__v4si v) : v(v) {}
	
	int32_t &operator[](int i) { return e[i]; }
	const int32_t &operator[](int i) const { return e[i]; }
	operator __v4si() { return v; }
	operator const __v4si() const { return v; }
	
	/*
	MLVec4I32 &operator+=(const MLVec4I32 &other) { v += other.v; return *this; }
	MLVec4I32 &operator-=(const MLVec4I32 &other) { v -= other.v; return *this; }
	MLVec4I32 &operator*=(const MLVec4I32 &other) { v *= other.v; return *this; }
	MLVec4I32 &operator/=(const MLVec4I32 &other) { v /= other.v; return *this; }
	
	MLVec4I32 &operator+=(int32_t value) { v += MLVec4I32(value).v; return *this; }
	MLVec4I32 &operator-=(int32_t value) { v -= MLVec4I32(value).v; return *this; }
	MLVec4I32 &operator*=(int32_t value) { v *= MLVec4I32(value).v; return *this; }
	MLVec4I32 &operator/=(int32_t value) { v /= MLVec4I32(value).v; return *this; }
	
	const MLVec4I32 operator+(const MLVec4I32 &other) const { MLVec4I32 r; r.v = v + other.v; return r; }
	const MLVec4I32 operator-(const MLVec4I32 &other) const { MLVec4I32 r; r.v = v - other.v; return r; }
	const MLVec4I32 operator*(const MLVec4I32 &other) const { MLVec4I32 r; r.v = v * other.v; return r; }
	const MLVec4I32 operator/(const MLVec4I32 &other) const { MLVec4I32 r; r.v = v / other.v; return r; }
	*/
	
	union
	{
		struct
		{
			int32_t x;
			int32_t y;
			int32_t z;
			int32_t w;
		};
		struct
		{
			int32_t b;
			int32_t g;
			int32_t r;
			int32_t a;
		};
		int32_t e[4];
		__v4si v;
	};
};

struct MLVec4U16
{
	MLVec4U16() {}
	MLVec4U16(uint16_t s)
	{
		e[0] = s;
		e[1] = s;
		e[2] = s;
		e[3] = s;
	}
	MLVec4U16(uint16_t e0, uint16_t e1, uint16_t e2, uint16_t e3)
	{
		e[0] = e0;
		e[1] = e1;
		e[2] = e2;
		e[3] = e3;
	}
	
	uint16_t &operator[](int i) { return e[i]; }
	const uint16_t &operator[](int i) const { return e[i]; }
	operator const uint64_t &() { return data; }
	
	union
	{
		struct
		{
			uint16_t x;
			uint16_t y;
			uint16_t z;
			uint16_t w;
		};
		struct
		{
			uint16_t b;
			uint16_t g;
			uint16_t r;
			uint16_t a;
		};
		uint16_t e[4];
		uint64_t data;
	};
};

struct MLVec4U8
{
	MLVec4U8() {}
	MLVec4U8(uint8_t s)
	{
		e[0] = s;
		e[1] = s;
		e[2] = s;
		e[3] = s;
	}
	MLVec4U8(uint8_t e0, uint8_t e1, uint8_t e2, uint8_t e3)
	{
		e[0] = e0;
		e[1] = e1;
		e[2] = e2;
		e[3] = e3;
	}
	
	uint8_t &operator[](int i) { return e[i]; }
	const uint8_t &operator[](int i) const { return e[i]; }
	operator const uint32_t &() { return data; }
	
	union
	{
		struct
		{
			uint8_t x;
			uint8_t y;
			uint8_t z;
			uint8_t w;
		};
		struct
		{
			uint8_t b;
			uint8_t g;
			uint8_t r;
			uint8_t a;
		};
		uint8_t e[4];
		uint32_t data;
	};
};

struct MLVec3U16
{
	MLVec3U16() {}
	MLVec3U16(uint16_t s)
	{
		e[0] = s;
		e[1] = s;
		e[2] = s;
	}
	MLVec3U16(uint16_t e0, uint16_t e1, uint16_t e2)
	{
		e[0] = e0;
		e[1] = e1;
		e[2] = e2;
	}
	
	uint16_t &operator[](int i) { return e[i]; }
	const uint16_t &operator[](int i) const { return e[i]; }
	
	union
	{
		struct
		{
			uint16_t x;
			uint16_t y;
			uint16_t z;
		};
		struct
		{
			uint16_t b;
			uint16_t g;
			uint16_t r;
		};
		uint16_t e[3];
	};
};

struct MLVec3U8
{
	MLVec3U8() {}
	MLVec3U8(uint8_t s)
	{
		e[0] = s;
		e[1] = s;
		e[2] = s;
	}
	MLVec3U8(uint8_t e0, uint8_t e1, uint8_t e2)
	{
		e[0] = e0;
		e[1] = e1;
		e[2] = e2;
	}
	
	uint8_t &operator[](int i) { return e[i]; }
	const uint8_t &operator[](int i) const { return e[i]; }
	
	union
	{
		struct
		{
			uint8_t x;
			uint8_t y;
			uint8_t z;
		};
		struct
		{
			uint8_t b;
			uint8_t g;
			uint8_t r;
		};
		uint8_t e[3];
	};
};

struct MLVec8I16
{
	ML_ALIGN_16BYTE
	
	MLVec8I16() {}
	MLVec8I16(int16_t s)
	{
		e[0] = s;
		e[1] = s;
		e[2] = s;
		e[3] = s;
		e[4] = s;
		e[5] = s;
		e[6] = s;
		e[7] = s;
	}
	MLVec8I16(int16_t e0, int16_t e1, int16_t e2, int16_t e3,
			  int16_t e4, int16_t e5, int16_t e6, int16_t e7)
	{
		e[0] = e0;
		e[1] = e1;
		e[2] = e2;
		e[3] = e3;
		e[4] = e4;
		e[5] = e5;
		e[6] = e6;
		e[7] = e7;
	}
	MLVec8I16(__v8hi v) : v(v) {}
	
	static MLVec8I16 pack(const MLVec4I32 &v0, const MLVec4I32 &v1)
	{
		return __builtin_ia32_packssdw128(v0, v1);
	}
	
	int16_t &operator[](int i) { return e[i]; }
	const int16_t &operator[](int i) const { return e[i]; }
	operator __v8hi() { return v; }
	operator const __v8hi() const { return v; }
	
	/*
	MLVec8I16 &operator+=(const MLVec8I16 &other) { v += other.v; return *this; }
	MLVec8I16 &operator-=(const MLVec8I16 &other) { v -= other.v; return *this; }
	MLVec8I16 &operator*=(const MLVec8I16 &other) { v *= other.v; return *this; }
	MLVec8I16 &operator/=(const MLVec8I16 &other) { v /= other.v; return *this; }
	
	MLVec8I16 &operator+=(int16_t value) { v += MLVec8I16(value).v; return *this; }
	MLVec8I16 &operator-=(int16_t value) { v -= MLVec8I16(value).v; return *this; }
	MLVec8I16 &operator*=(int16_t value) { v *= MLVec8I16(value).v; return *this; }
	MLVec8I16 &operator/=(int16_t value) { v /= MLVec8I16(value).v; return *this; }
	
	const MLVec8I16 operator+(const MLVec8I16 &other) const { MLVec8I16 r; r.v = v + other.v; return r; }
	const MLVec8I16 operator-(const MLVec8I16 &other) const { MLVec8I16 r; r.v = v - other.v; return r; }
	const MLVec8I16 operator*(const MLVec8I16 &other) const { MLVec8I16 r; r.v = v * other.v; return r; }
	const MLVec8I16 operator/(const MLVec8I16 &other) const { MLVec8I16 r; r.v = v / other.v; return r; }
	*/
	
	union
	{
		int16_t e[8];
		__v8hi v;
	};
};

struct MLVec16I8
{
	ML_ALIGN_16BYTE
	
	MLVec16I8() {}
	MLVec16I8(int8_t s)
	{
		e[0] = s;
		e[1] = s;
		e[2] = s;
		e[3] = s;
		e[4] = s;
		e[5] = s;
		e[6] = s;
		e[7] = s;
		e[8] = s;
		e[9] = s;
		e[10] = s;
		e[11] = s;
		e[12] = s;
		e[13] = s;
		e[14] = s;
		e[15] = s;
	}
	MLVec16I8(int8_t e0, int8_t e1, int8_t e2, int8_t e3,
			  int8_t e4, int8_t e5, int8_t e6, int8_t e7,
			  int8_t e8, int8_t e9, int8_t e10, int8_t e11,
			  int8_t e12, int8_t e13, int8_t e14, int8_t e15)
	{
		e[0] = e0;
		e[1] = e1;
		e[2] = e2;
		e[3] = e3;
		e[4] = e4;
		e[5] = e5;
		e[6] = e6;
		e[7] = e7;
		e[8] = e8;
		e[9] = e9;
		e[10] = e10;
		e[11] = e11;
		e[12] = e12;
		e[13] = e13;
		e[14] = e14;
		e[15] = e15;
	}
	MLVec16I8(__v16qi v) : v(v) {}
	
	static MLVec16I8 pack(const MLVec8I16 &v0, const MLVec8I16 &v1)
	{
		return MLVec16I8(__builtin_ia32_packsswb128(v0, v1));
	}
	
	int8_t &operator[](int i) { return e[i]; }
	const int8_t &operator[](int i) const { return e[i]; }
	operator __v16qi() { return v; }
	operator const __v16qi() const { return v; }
	
	/*
	MLVec16I8 &operator+=(const MLVec16I8 &other) { v += other.v; return *this; }
	MLVec16I8 &operator-=(const MLVec16I8 &other) { v -= other.v; return *this; }
	MLVec16I8 &operator*=(const MLVec16I8 &other) { v *= other.v; return *this; }
	MLVec16I8 &operator/=(const MLVec16I8 &other) { v /= other.v; return *this; }
	
	MLVec16I8 &operator+=(int8_t value) { v += MLVec16I8(value).v; return *this; }
	MLVec16I8 &operator-=(int8_t value) { v -= MLVec16I8(value).v; return *this; }
	MLVec16I8 &operator*=(int8_t value) { v *= MLVec16I8(value).v; return *this; }
	MLVec16I8 &operator/=(int8_t value) { v /= MLVec16I8(value).v; return *this; }
	
	const MLVec16I8 operator+(const MLVec16I8 &other) const { MLVec16I8 r; r.v = v + other.v; return r; }
	const MLVec16I8 operator-(const MLVec16I8 &other) const { MLVec16I8 r; r.v = v - other.v; return r; }
	const MLVec16I8 operator*(const MLVec16I8 &other) const { MLVec16I8 r; r.v = v * other.v; return r; }
	const MLVec16I8 operator/(const MLVec16I8 &other) const { MLVec16I8 r; r.v = v / other.v; return r; }
	*/
	
	union
	{
		int8_t e[16];
		__v16qi v;
	};
};

struct MLVec16U8
{
	ML_ALIGN_16BYTE
	
	MLVec16U8() {}
	MLVec16U8(uint8_t s)
	{
		e[0] = s;
		e[1] = s;
		e[2] = s;
		e[3] = s;
		e[4] = s;
		e[5] = s;
		e[6] = s;
		e[7] = s;
		e[8] = s;
		e[9] = s;
		e[10] = s;
		e[11] = s;
		e[12] = s;
		e[13] = s;
		e[14] = s;
		e[15] = s;
	}
	MLVec16U8(uint8_t e0, uint8_t e1, uint8_t e2, uint8_t e3,
			  uint8_t e4, uint8_t e5, uint8_t e6, uint8_t e7,
			  uint8_t e8, uint8_t e9, uint8_t e10, uint8_t e11,
			  uint8_t e12, uint8_t e13, uint8_t e14, uint8_t e15)
	{
		e[0] = e0;
		e[1] = e1;
		e[2] = e2;
		e[3] = e3;
		e[4] = e4;
		e[5] = e5;
		e[6] = e6;
		e[7] = e7;
		e[8] = e8;
		e[9] = e9;
		e[10] = e10;
		e[11] = e11;
		e[12] = e12;
		e[13] = e13;
		e[14] = e14;
		e[15] = e15;
	}
	MLVec16U8(__v16qi v) : v(v) {}
	
	static MLVec16U8 pack(const MLVec8I16 &v0, const MLVec8I16 &v1)
	{
		return MLVec16U8(__builtin_ia32_packuswb128(v0, v1));
	}
	
	uint8_t &operator[](int i) { return e[i]; }
	const uint8_t &operator[](int i) const { return e[i]; }
	operator __v16qi() { return v; }
	operator const __v16qi() const { return v; }
	
	union
	{
		uint8_t e[16];
		__v16qi v;
	};
};

struct MLVec4F
{
	ML_ALIGN_16BYTE
	
	MLVec4F() {}
	MLVec4F(float s) { e[0] = s; e[1] = s; e[2] = s; e[3] = s; }
	MLVec4F(float e0, float e1, float e2, float e3) { e[0] = e0; e[1] = e1; e[2] = e2; e[3] = e3; }
	MLVec4F(__v4sf v) : v(v) {}
	
	static MLVec4I32 equal(const MLVec4F &v0, const MLVec4F &v1) { return MLVec4I32((__v4si)__builtin_ia32_cmpeqps(v0, v1)); }
	static MLVec4I32 notEqual(const MLVec4F &v0, const MLVec4F &v1) { return MLVec4I32((__v4si)__builtin_ia32_cmpneqps(v0, v1)); }
	static MLVec4I32 lessThan(const MLVec4F &v0, const MLVec4F &v1) { return MLVec4I32((__v4si)__builtin_ia32_cmpltps(v0, v1)); }
	static MLVec4I32 lessThanOrEqual(const MLVec4F &v0, const MLVec4F &v1) { return MLVec4I32((__v4si)__builtin_ia32_cmpleps(v0, v1)); }
	static MLVec4I32 greaterThan(const MLVec4F &v0, const MLVec4F &v1) { return MLVec4I32((__v4si)__builtin_ia32_cmpltps(v1, v0)); }
	static MLVec4I32 greaterThanOrEqual(const MLVec4F &v0, const MLVec4F &v1) { return MLVec4I32((__v4si)__builtin_ia32_cmpleps(v1, v0)); }
	
	float &operator[](int i) { return e[i]; }
	const float &operator[](int i) const { return e[i]; }
	operator __v4sf() { return v; }
	operator const __v4sf() const { return v; }
	
	MLVec4F &operator+=(const MLVec4F &other) { v += other.v; return *this; }
	MLVec4F &operator-=(const MLVec4F &other) { v -= other.v; return *this; }
	MLVec4F &operator*=(const MLVec4F &other) { v *= other.v; return *this; }
	MLVec4F &operator/=(const MLVec4F &other) { v /= other.v; return *this; }
	
	MLVec4F &operator+=(float value) { v += MLVec4F(value).v; return *this; }
	MLVec4F &operator-=(float value) { v -= MLVec4F(value).v; return *this; }
	MLVec4F &operator*=(float value) { v *= MLVec4F(value).v; return *this; }
	MLVec4F &operator/=(float value) { v /= MLVec4F(value).v; return *this; }
	
	const MLVec4F operator+(const MLVec4F &other) const { MLVec4F r; r.v = v + other.v; return r; }
	const MLVec4F operator-(const MLVec4F &other) const { MLVec4F r; r.v = v - other.v; return r; }
	const MLVec4F operator*(const MLVec4F &other) const { MLVec4F r; r.v = v * other.v; return r; }
	const MLVec4F operator/(const MLVec4F &other) const { MLVec4F r; r.v = v / other.v; return r; }
	
	MLVec4F &operator=(const MLVec4I32 &iv) { v = __builtin_ia32_cvtdq2ps(iv.v); return *this; }
	
	union
	{
		struct
		{
			float x;
			float y;
			float z;
			float w;
		};
		struct
		{
			float b;
			float g;
			float r;
			float a;
		};
		float e[4];
		__v4sf v;
	};
};

inline const MLVec4F operator+(const MLVec4F &x, float s) { return x + MLVec4F(s); }
inline const MLVec4F operator-(const MLVec4F &x, float s) { return x - MLVec4F(s); }
inline const MLVec4F operator*(const MLVec4F &x, float s) { return x * MLVec4F(s); }
inline const MLVec4F operator/(const MLVec4F &x, float s) { return x / MLVec4F(s); }

inline const MLVec4F operator+(float s, const MLVec4F &x) { return MLVec4F(s) + x; }
inline const MLVec4F operator-(float s, const MLVec4F &x) { return MLVec4F(s) - x; }
inline const MLVec4F operator*(float s, const MLVec4F &x) { return MLVec4F(s) * x; }
inline const MLVec4F operator/(float s, const MLVec4F &x) { return MLVec4F(s) / x; }

struct MLVec2D
{
	ML_ALIGN_16BYTE
	
	MLVec2D() {}
	MLVec2D(double s) : x(s), y(s) {}
	MLVec2D(double x, double y) : x(x), y(y) {}
	MLVec2D(const QPointF &p) : x(p.x()), y(p.y()) {}
	MLVec2D(__v2df v) : v(v) {}
	
	QPoint toQPoint() const { return QPointF(*this).toPoint(); }
	
	operator const QPointF&() const { return *reinterpret_cast<const QPointF *>(this); }
	operator const QSizeF&() const { return *reinterpret_cast<const QSizeF *>(this); }
	
	static MLVec2I64 equal(const MLVec2D &v0, const MLVec2D &v1) { return MLVec2I64((__v2di)__builtin_ia32_cmpeqpd(v0, v1)); }
	static MLVec2I64 notEqual(const MLVec2D &v0, const MLVec2D &v1) { return MLVec2I64((__v2di)__builtin_ia32_cmpneqpd(v0, v1)); }
	static MLVec2I64 lessThan(const MLVec2D &v0, const MLVec2D &v1) { return MLVec2I64((__v2di)__builtin_ia32_cmpltpd(v0, v1)); }
	static MLVec2I64 lessThanOrEqual(const MLVec2D &v0, const MLVec2D &v1) { return MLVec2I64((__v2di)__builtin_ia32_cmplepd(v0, v1)); }
	static MLVec2I64 greaterThan(const MLVec2D &v0, const MLVec2D &v1) { return MLVec2I64((__v2di)__builtin_ia32_cmpltpd(v1, v0)); }
	static MLVec2I64 greaterThanOrEqual(const MLVec2D &v0, const MLVec2D &v1) { return MLVec2I64((__v2di)__builtin_ia32_cmplepd(v1, v0)); }
	
	double &operator[](int i) { return e[i]; }
	const double &operator[](int i) const { return e[i]; }
	operator __v2df() { return v; }
	operator const __v2df() const { return v; }
	
	MLVec2D &operator+=(const MLVec2D &other) { v += other.v; return *this; }
	MLVec2D &operator-=(const MLVec2D &other) { v -= other.v; return *this; }
	MLVec2D &operator*=(const MLVec2D &other) { v *= other.v; return *this; }
	MLVec2D &operator/=(const MLVec2D &other) { v /= other.v; return *this; }
	
	MLVec2D &operator+=(double value) { v += MLVec2D(value).v; return *this; }
	MLVec2D &operator-=(double value) { v -= MLVec2D(value).v; return *this; }
	MLVec2D &operator*=(double value) { v *= MLVec2D(value).v; return *this; }
	MLVec2D &operator/=(double value) { v /= MLVec2D(value).v; return *this; }
	
	const MLVec2D operator+(const MLVec2D &other) const { MLVec2D r; r.v = v + other.v; return r; }
	const MLVec2D operator-(const MLVec2D &other) const { MLVec2D r; r.v = v - other.v; return r; }
	const MLVec2D operator*(const MLVec2D &other) const { MLVec2D r; r.v = v * other.v; return r; }
	const MLVec2D operator/(const MLVec2D &other) const { MLVec2D r; r.v = v / other.v; return r; }
	
	inline MLVec2D &operator*=(const QTransform &transform);
	
	union
	{
		struct
		{
			double x;
			double y;
		};
		double e[2];
		__v2df v;
	};
};

inline bool operator==(const MLVec2D &v0, const MLVec2D &v1) { return v0.x == v1.x && v0.y == v1.y; }
inline bool operator!=(const MLVec2D &v0, const MLVec2D &v1) { return v0.x != v1.x || v0.y != v1.y; }

inline const MLVec2D operator+(const MLVec2D &x, double s) { return x + MLVec2D(s); }
inline const MLVec2D operator-(const MLVec2D &x, double s) { return x - MLVec2D(s); }
inline const MLVec2D operator*(const MLVec2D &x, double s) { return x * MLVec2D(s); }
inline const MLVec2D operator/(const MLVec2D &x, double s) { return x / MLVec2D(s); }

inline const MLVec2D operator+(double s, const MLVec2D &x) { return MLVec2D(s) + x; }
inline const MLVec2D operator-(double s, const MLVec2D &x) { return MLVec2D(s) - x; }
inline const MLVec2D operator*(double s, const MLVec2D &x) { return MLVec2D(s) * x; }
inline const MLVec2D operator/(double s, const MLVec2D &x) { return MLVec2D(s) / x; }

inline MLVec2D operator*(const MLVec2D &v, const QTransform &transform)
{
	MLVec2D r;
	transform.map(v.x, v.y, &(r.x), &(r.y));
	return r;
}

inline MLVec2D &MLVec2D::operator*=(const QTransform &transform)
{
	return *this = *this * transform;
}

inline MLVec4F mlMax(const MLVec4F &v1, const MLVec4F &v2) { return MLVec4F(__builtin_ia32_maxps(v1, v2)); }
inline MLVec4F mlMin(const MLVec4F &v1, const MLVec4F &v2) { return MLVec4F(__builtin_ia32_minps(v1, v2)); }

inline MLVec4F mlBound(const MLVec4F &min, const MLVec4F &v, const MLVec4F &max)
{
	MLVec4F r;
	r = mlMax(min, v);
	r = mlMin(r, max);
	return r;
}

inline MLVec4I32 mlRound(const MLVec4F &v) { return MLVec4I32(__builtin_ia32_cvtps2dq(v)); }
inline MLVec4I32 mlRound(const MLVec2D &v) { return MLVec4I32(__builtin_ia32_cvtpd2dq(v)); }

inline MLVec4F mlSqrt(const MLVec4F &v) { return MLVec4F(__builtin_ia32_sqrtps(v)); }
inline MLVec2D mlSqrt(const MLVec2D &v) { return MLVec2D(__builtin_ia32_sqrtpd(v)); }

inline double mlDot(const MLVec2D &v1, const MLVec2D & v2)
{
	MLVec2D v = v1 * v2;
	return v.x + v.y;
}

inline double mlSqLength(const MLVec2D &v)
{
	return mlDot(v, v);
}

inline double mlLength(const MLVec2D &v)
{
	return sqrt(mlSqLength(v));
}

inline double mlArg(const MLVec2D &v)
{
	return atan2(v.x, v.y);
}

Q_DECLARE_METATYPE(MLVec2I64)
Q_DECLARE_METATYPE(MLVec4I32)
Q_DECLARE_METATYPE(MLVec8I16)
Q_DECLARE_METATYPE(MLVec16I8)
Q_DECLARE_METATYPE(MLVec16U8)
Q_DECLARE_METATYPE(MLVec2D)
Q_DECLARE_METATYPE(MLVec4F)
Q_DECLARE_METATYPE(MLVec4U16)
Q_DECLARE_METATYPE(MLVec4U8)
Q_DECLARE_METATYPE(MLVec3U16)
Q_DECLARE_METATYPE(MLVec3U8)


#endif // MLVECTOR_H
