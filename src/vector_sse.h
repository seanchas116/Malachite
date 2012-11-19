#ifndef VECTOR_SSE_H
#define VECTOR_SSE_H

#include <stdint.h>
#include <emmintrin.h>
#include "vector_generic.h"

namespace Malachite
{

template<>
class Vector<float, 4>
{
public:
	
	typedef float value_type;
	typedef size_t size_type;
	constexpr static size_t size() { return 4; }
	
	Vector() {}
	Vector(float s)
	{
		_array[0] = s;
		_data = _mm_unpacklo_ps(_data, _data);
		_data = _mm_unpacklo_ps(_data, _data);
	}

	Vector(std::initializer_list<float> list)
	{
		auto iter = list.begin();
		_array[0] = *iter++;
		_array[1] = *iter++;
		_array[2] = *iter++;
		_array[3] = *iter++;
	}
	
	Vector(__m128 data) { _data = data; }
	
	// access
	
	template <size_t T_Index> float &e() { return _array[T_Index]; }
	template <size_t T_Index> const float &e() const { return _array[T_Index]; }
	
	template <size_t T_Index> Vector extract() const
	{
		switch (T_Index)
		{
			default:
			case 0:
			{
				__m128 rd = _mm_unpacklo_ps(_data, _data);
				rd = _mm_unpacklo_ps(rd, rd);
				return rd;
			}
			case 1:
			{
				__m128 rd = _mm_unpacklo_ps(_data, _data);
				rd = _mm_unpackhi_ps(rd, rd);
				return rd;
			}
			case 2:
			{
				__m128 rd = _mm_unpackhi_ps(_data, _data);
				rd = _mm_unpacklo_ps(rd, rd);
				return rd;
			}
			case 3:
			{
				__m128 rd = _mm_unpackhi_ps(_data, _data);
				rd = _mm_unpackhi_ps(rd, rd);
				return rd;
			}
		}
	}
	
	float &operator[](size_t index) { return _array[index]; }
	const float &operator[](size_t index) const { return  _array[index]; }
	
	// arithmetic
	
	Vector &operator+=(const Vector &other) { _v += other._v; return *this; }
	Vector &operator-=(const Vector &other) { _v -= other._v; return *this; }
	Vector &operator*=(const Vector &other) { _v *= other._v; return *this; }
	Vector &operator/=(const Vector &other) { _v /= other._v; return *this; }
	
	Vector &operator+=(float value) { return operator+=(Vector(value)); }
	Vector &operator-=(float value) { return operator-=(Vector(value)); }
	Vector &operator*=(float value) { return operator*=(Vector(value)); }
	Vector &operator/=(float value) { return operator/=(Vector(value)); }
	
	const Vector operator+(const Vector &other) { Vector ret; ret += other; return ret; }
	const Vector operator-(const Vector &other) { Vector ret; ret -= other; return ret; }
	const Vector operator*(const Vector &other) { Vector ret; ret *= other; return ret; }
	const Vector operator/(const Vector &other) { Vector ret; ret /= other; return ret; }
	
	// comparison
	
	class ComparisonResult
	{
	public:
		
		ComparisonResult(__m128 data) { _data = data; }
		
		template <size_t T_Index> uint32_t &e() { return _array[T_Index]; }
		template <size_t T_Index> const uint32_t &e() const { return _array[T_Index]; }
		
		uint32_t &operator[](size_t index) { return _array[index]; }
		const uint32_t &operator[](size_t index) const { return  _array[index]; }
		
		__m128 &data() { return _data; }
		const __m128 &data() const { return _data; }
		operator __m128() const { return _data; }
		
	private:
		
		union
		{
			__m128 _data;
			uint32_t _array[4];
		};
	};
	
	ComparisonResult operator<(const Vector &other) const { return _mm_cmplt_ps(_data, other._data); }
	ComparisonResult operator<=(const Vector &other) const { return _mm_cmple_ps(_data, other._data); }
	ComparisonResult operator>(const Vector &other) const { return _mm_cmpgt_ps(_data, other._data); }
	ComparisonResult operator>=(const Vector &other) const { return _mm_cmpge_ps(_data, other._data); }
	ComparisonResult operator==(const Vector &other) const { return _mm_cmpeq_ps(_data, other._data); }
	ComparisonResult operator!=(const Vector &other) const { return _mm_cmpneq_ps(_data, other._data); }
	
	// original
	
	__m128 &data() { return _data; }
	const __m128 &data() const { return _data; }
	operator __m128() const { return _data; }
	
protected:
	
	union
	{
		__m128 _data;
		__v4sf _v;
		float _array[4];
	};
};

}

#endif // VECTOR_SSE_H
