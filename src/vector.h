#ifndef MLVECTOR_H
#define MLVECTOR_H

//ExportName: Vector

#include <stdint.h>
#include <emmintrin.h>
#include <cstdlib>
#include <cmath>
#include <new>
#include <QPointF>
#include <QMetaType>
#include <QTransform>

#include "global.h"
#include "memory.h"
#include "misc.h"

namespace Malachite
{

struct Vec2I64
{
	ML_ALIGN_16BYTE
	
	Vec2I64() {}
	Vec2I64(int64_t s)
	{
		e[0] = s;
		e[1] = s;
	}
	Vec2I64(int64_t e0, int64_t e1)
	{
		e[0] = e0;
		e[1] = e1;
	}
	Vec2I64(__v2di v) : v(v) {}
	
	int64_t &operator[](int i) { return e[i]; }
	const int64_t &operator[](int i) const { return e[i]; }
	operator __v2di() { return v; }
	operator const __v2di() const { return v; }
	
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

struct Vec4I32
{
	ML_ALIGN_16BYTE
	
	Vec4I32() {}
	Vec4I32(int32_t s)
	{
		e[0] = s;
		e[1] = s;
		e[2] = s;
		e[3] = s;
	}
	Vec4I32(int32_t e0, int32_t e1, int32_t e2, int32_t e3)
	{
		e[0] = e0;
		e[1] = e1;
		e[2] = e2;
		e[3] = e3;
	}
	Vec4I32(__v4si v) : v(v) {}
	
	int32_t &operator[](int i) { return e[i]; }
	const int32_t &operator[](int i) const { return e[i]; }
	operator __v4si() { return v; }
	operator const __v4si() const { return v; }
	
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

struct Vec4U16
{
	Vec4U16() {}
	Vec4U16(uint16_t s)
	{
		e[0] = s;
		e[1] = s;
		e[2] = s;
		e[3] = s;
	}
	Vec4U16(uint16_t e0, uint16_t e1, uint16_t e2, uint16_t e3)
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

struct Vec4U8
{
	Vec4U8() {}
	Vec4U8(uint8_t s)
	{
		e[0] = s;
		e[1] = s;
		e[2] = s;
		e[3] = s;
	}
	Vec4U8(uint8_t e0, uint8_t e1, uint8_t e2, uint8_t e3)
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

struct Vec3U16
{
	Vec3U16() {}
	Vec3U16(uint16_t s)
	{
		e[0] = s;
		e[1] = s;
		e[2] = s;
	}
	Vec3U16(uint16_t e0, uint16_t e1, uint16_t e2)
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

struct Vec3U8
{
	Vec3U8() {}
	Vec3U8(uint8_t s)
	{
		e[0] = s;
		e[1] = s;
		e[2] = s;
	}
	Vec3U8(uint8_t e0, uint8_t e1, uint8_t e2)
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

struct Vec8I16
{
	ML_ALIGN_16BYTE
	
	Vec8I16() {}
	Vec8I16(int16_t s)
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
	Vec8I16(int16_t e0, int16_t e1, int16_t e2, int16_t e3,
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
	Vec8I16(__v8hi v) : v(v) {}
	
	static Vec8I16 pack(const Vec4I32 &v0, const Vec4I32 &v1)
	{
		return __builtin_ia32_packssdw128(v0, v1);
	}
	
	int16_t &operator[](int i) { return e[i]; }
	const int16_t &operator[](int i) const { return e[i]; }
	operator __v8hi() { return v; }
	operator const __v8hi() const { return v; }
	
	union
	{
		int16_t e[8];
		__v8hi v;
	};
};

struct Vec16I8
{
	ML_ALIGN_16BYTE
	
	Vec16I8() {}
	Vec16I8(int8_t s)
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
	Vec16I8(int8_t e0, int8_t e1, int8_t e2, int8_t e3,
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
	Vec16I8(__v16qi v) : v(v) {}
	
	static Vec16I8 pack(const Vec8I16 &v0, const Vec8I16 &v1)
	{
		return Vec16I8(__builtin_ia32_packsswb128(v0, v1));
	}
	
	int8_t &operator[](int i) { return e[i]; }
	const int8_t &operator[](int i) const { return e[i]; }
	operator __v16qi() { return v; }
	operator const __v16qi() const { return v; }
	
	union
	{
		int8_t e[16];
		__v16qi v;
	};
};

struct Vec16U8
{
	ML_ALIGN_16BYTE
	
	Vec16U8() {}
	Vec16U8(uint8_t s)
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
	Vec16U8(uint8_t e0, uint8_t e1, uint8_t e2, uint8_t e3,
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
	Vec16U8(__v16qi v) : v(v) {}
	
	static Vec16U8 pack(const Vec8I16 &v0, const Vec8I16 &v1)
	{
		return Vec16U8(__builtin_ia32_packuswb128(v0, v1));
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

struct Vec4F
{
	ML_ALIGN_16BYTE
	
	Vec4F() {}
	//Vec4F(float s) { e[0] = s; e[1] = s; e[2] = s; e[3] = s; }
	Vec4F(float s)
	{
		e[0] = s;
		*this = extract0();
	}

	Vec4F(float e0, float e1, float e2, float e3) { e[0] = e0; e[1] = e1; e[2] = e2; e[3] = e3; }
	Vec4F(__v4sf v) : v(v) {}
	
	static Vec4I32 equal(const Vec4F &v0, const Vec4F &v1) { return Vec4I32((__v4si)__builtin_ia32_cmpeqps(v0, v1)); }
	static Vec4I32 notEqual(const Vec4F &v0, const Vec4F &v1) { return Vec4I32((__v4si)__builtin_ia32_cmpneqps(v0, v1)); }
	static Vec4I32 lessThan(const Vec4F &v0, const Vec4F &v1) { return Vec4I32((__v4si)__builtin_ia32_cmpltps(v0, v1)); }
	static Vec4I32 lessThanOrEqual(const Vec4F &v0, const Vec4F &v1) { return Vec4I32((__v4si)__builtin_ia32_cmpleps(v0, v1)); }
	static Vec4I32 greaterThan(const Vec4F &v0, const Vec4F &v1) { return Vec4I32((__v4si)__builtin_ia32_cmpltps(v1, v0)); }
	static Vec4I32 greaterThanOrEqual(const Vec4F &v0, const Vec4F &v1) { return Vec4I32((__v4si)__builtin_ia32_cmpleps(v1, v0)); }
	
	float &operator[](size_t i) { return e[i]; }
	const float &operator[](size_t i) const { return e[i]; }
	operator __v4sf() { return v; }
	operator const __v4sf() const { return v; }
	
	Vec4F extract0() const
	{
		__m128 rd = __builtin_ia32_unpcklps(v, v);
		rd = __builtin_ia32_unpcklps(rd, rd);
		return rd;
	}
	
	Vec4F extract1() const
	{
		__m128 rd = __builtin_ia32_unpcklps(v, v);
		rd = __builtin_ia32_unpckhps(rd, rd);
		return rd;
	}
	
	Vec4F extract2() const
	{
		__m128 rd = __builtin_ia32_unpckhps(v, v);
		rd = __builtin_ia32_unpcklps(rd, rd);
		return rd;
	}
	
	Vec4F extract3() const
	{
		__m128 rd = __builtin_ia32_unpckhps(v, v);
		rd = __builtin_ia32_unpckhps(rd, rd);
		return rd;
	}
	
	Vec4F &operator+=(const Vec4F &other) { v += other.v; return *this; }
	Vec4F &operator-=(const Vec4F &other) { v -= other.v; return *this; }
	Vec4F &operator*=(const Vec4F &other) { v *= other.v; return *this; }
	Vec4F &operator/=(const Vec4F &other) { v /= other.v; return *this; }
	
	Vec4F &operator+=(float value) { v += Vec4F(value).v; return *this; }
	Vec4F &operator-=(float value) { v -= Vec4F(value).v; return *this; }
	Vec4F &operator*=(float value) { v *= Vec4F(value).v; return *this; }
	Vec4F &operator/=(float value) { v /= Vec4F(value).v; return *this; }
	
	const Vec4F operator+(const Vec4F &other) const { Vec4F r; r.v = v + other.v; return r; }
	const Vec4F operator-(const Vec4F &other) const { Vec4F r; r.v = v - other.v; return r; }
	const Vec4F operator*(const Vec4F &other) const { Vec4F r; r.v = v * other.v; return r; }
	const Vec4F operator/(const Vec4F &other) const { Vec4F r; r.v = v / other.v; return r; }
	
	Vec4F &operator=(const Vec4I32 &iv) { v = __builtin_ia32_cvtdq2ps(iv.v); return *this; }
	
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

inline const Vec4F operator+(const Vec4F &x, float s) { return x + Vec4F(s); }
inline const Vec4F operator-(const Vec4F &x, float s) { return x - Vec4F(s); }
inline const Vec4F operator*(const Vec4F &x, float s) { return x * Vec4F(s); }
inline const Vec4F operator/(const Vec4F &x, float s) { return x / Vec4F(s); }

inline const Vec4F operator+(float s, const Vec4F &x) { return Vec4F(s) + x; }
inline const Vec4F operator-(float s, const Vec4F &x) { return Vec4F(s) - x; }
inline const Vec4F operator*(float s, const Vec4F &x) { return Vec4F(s) * x; }
inline const Vec4F operator/(float s, const Vec4F &x) { return Vec4F(s) / x; }

struct Vec2D
{
	ML_ALIGN_16BYTE
	
	Vec2D() {}
	Vec2D(double s) : x(s), y(s) {}
	Vec2D(double x, double y) : x(x), y(y) {}
	Vec2D(const QPointF &p) : x(p.x()), y(p.y()) {}
	Vec2D(const QPoint &p) : x(p.x()), y(p.y()) {}
	Vec2D(__v2df v) : v(v) {}
	
	QPoint toQPoint() const { return QPointF(*this).toPoint(); }
	
	operator const QPointF&() const { return *reinterpret_cast<const QPointF *>(this); }
	operator const QSizeF&() const { return *reinterpret_cast<const QSizeF *>(this); }
	
	static Vec2I64 equal(const Vec2D &v0, const Vec2D &v1) { return Vec2I64((__v2di)__builtin_ia32_cmpeqpd(v0, v1)); }
	static Vec2I64 notEqual(const Vec2D &v0, const Vec2D &v1) { return Vec2I64((__v2di)__builtin_ia32_cmpneqpd(v0, v1)); }
	static Vec2I64 lessThan(const Vec2D &v0, const Vec2D &v1) { return Vec2I64((__v2di)__builtin_ia32_cmpltpd(v0, v1)); }
	static Vec2I64 lessThanOrEqual(const Vec2D &v0, const Vec2D &v1) { return Vec2I64((__v2di)__builtin_ia32_cmplepd(v0, v1)); }
	static Vec2I64 greaterThan(const Vec2D &v0, const Vec2D &v1) { return Vec2I64((__v2di)__builtin_ia32_cmpltpd(v1, v0)); }
	static Vec2I64 greaterThanOrEqual(const Vec2D &v0, const Vec2D &v1) { return Vec2I64((__v2di)__builtin_ia32_cmplepd(v1, v0)); }
	
	double &operator[](size_t i) { return e[i]; }
	const double &operator[](size_t i) const { return e[i]; }
	operator __v2df() { return v; }
	operator const __v2df() const { return v; }
	
	Vec2D extract0() const
	{
		return __builtin_ia32_unpcklpd(v, v);
	}
	
	Vec2D extract1() const
	{
		return __builtin_ia32_unpckhpd(v, v);
	}
	
	Vec2D &operator+=(const Vec2D &other) { v += other.v; return *this; }
	Vec2D &operator-=(const Vec2D &other) { v -= other.v; return *this; }
	Vec2D &operator*=(const Vec2D &other) { v *= other.v; return *this; }
	Vec2D &operator/=(const Vec2D &other) { v /= other.v; return *this; }
	
	Vec2D &operator+=(double value) { v += Vec2D(value).v; return *this; }
	Vec2D &operator-=(double value) { v -= Vec2D(value).v; return *this; }
	Vec2D &operator*=(double value) { v *= Vec2D(value).v; return *this; }
	Vec2D &operator/=(double value) { v /= Vec2D(value).v; return *this; }
	
	const Vec2D operator+(const Vec2D &other) const { Vec2D r; r.v = v + other.v; return r; }
	const Vec2D operator-(const Vec2D &other) const { Vec2D r; r.v = v - other.v; return r; }
	const Vec2D operator*(const Vec2D &other) const { Vec2D r; r.v = v * other.v; return r; }
	const Vec2D operator/(const Vec2D &other) const { Vec2D r; r.v = v / other.v; return r; }
	
	inline Vec2D &operator*=(const QTransform &transform);
	
	union
	{
		struct
		{
			double x;
			double y;
		};
		struct
		{
			double e1;
			double e2;
		};
		double e[2];
		__v2df v;
	};
};

inline bool operator==(const Vec2D &v0, const Vec2D &v1) { return v0.x == v1.x && v0.y == v1.y; }
inline bool operator!=(const Vec2D &v0, const Vec2D &v1) { return v0.x != v1.x || v0.y != v1.y; }

inline const Vec2D operator+(const Vec2D &x, double s) { return x + Vec2D(s); }
inline const Vec2D operator-(const Vec2D &x, double s) { return x - Vec2D(s); }
inline const Vec2D operator*(const Vec2D &x, double s) { return x * Vec2D(s); }
inline const Vec2D operator/(const Vec2D &x, double s) { return x / Vec2D(s); }

inline const Vec2D operator+(double s, const Vec2D &x) { return Vec2D(s) + x; }
inline const Vec2D operator-(double s, const Vec2D &x) { return Vec2D(s) - x; }
inline const Vec2D operator*(double s, const Vec2D &x) { return Vec2D(s) * x; }
inline const Vec2D operator/(double s, const Vec2D &x) { return Vec2D(s) / x; }

inline Vec2D operator*(const Vec2D &v, const QTransform &transform)
{
	Vec2D r;
	transform.map(v.x, v.y, &(r.x), &(r.y));
	return r;
}

inline Vec2D &Vec2D::operator*=(const QTransform &transform)
{
	return *this = *this * transform;
}

inline Vec4F vecMax(const Vec4F &v1, const Vec4F &v2) { return Vec4F(__builtin_ia32_maxps(v1, v2)); }
inline Vec4F vecMin(const Vec4F &v1, const Vec4F &v2) { return Vec4F(__builtin_ia32_minps(v1, v2)); }

inline Vec4F vecBound(const Vec4F &min, const Vec4F &v, const Vec4F &max)
{
	Vec4F r;
	r = vecMax(min, v);
	r = vecMin(r, max);
	return r;
}

inline Vec4I32 vecRound(const Vec4F &v) { return Vec4I32(__builtin_ia32_cvtps2dq(v)); }
inline Vec4I32 vecRound(const Vec2D &v) { return Vec4I32(__builtin_ia32_cvtpd2dq(v)); }

inline Vec4F vecSqrt(const Vec4F &v) { return Vec4F(__builtin_ia32_sqrtps(v)); }
inline Vec2D vecSqrt(const Vec2D &v) { return Vec2D(__builtin_ia32_sqrtpd(v)); }

inline double vecDot(const Vec2D &v1, const Vec2D & v2)
{
	Vec2D v = v1 * v2;
	return v.x + v.y;
}

inline double vecSqLength(const Vec2D &v)
{
	return vecDot(v, v);
}

inline double vecLength(const Vec2D &v)
{
	return sqrt(vecSqLength(v));
}

inline double vecArg(const Vec2D &v)
{
	return atan2(v.x, v.y);
}

inline Vec2D vecRotate(double r, double arg)
{
	return Vec2D(cos(arg), sin(arg)) * r;
}

inline Vec2D polarToCartesian(const Vec2D &v)
{
	return Vec2D(cos(v.y), sin(v.y)) * v.x;
}

inline Vec2D cartesianToPolar(const Vec2D &v)
{
	return Vec2D(vecLength(v), vecArg(v));
}

}

Q_DECLARE_METATYPE(Malachite::Vec2I64)
Q_DECLARE_METATYPE(Malachite::Vec4I32)
Q_DECLARE_METATYPE(Malachite::Vec8I16)
Q_DECLARE_METATYPE(Malachite::Vec16I8)
Q_DECLARE_METATYPE(Malachite::Vec16U8)
Q_DECLARE_METATYPE(Malachite::Vec2D)
Q_DECLARE_METATYPE(Malachite::Vec4F)
Q_DECLARE_METATYPE(Malachite::Vec4U16)
Q_DECLARE_METATYPE(Malachite::Vec4U8)
Q_DECLARE_METATYPE(Malachite::Vec3U16)
Q_DECLARE_METATYPE(Malachite::Vec3U8)

#endif // MLVECTOR_H
