#ifndef VECTOR_GENERIC_H
#define VECTOR_GENERIC_H

#include <initializer_list>
#include <array>
#include <cstddef>

namespace Malachite
{

template <class T, size_t N>
class Vector
{
public:
	
	typedef T value_type;
	typedef size_t size_type;
	constexpr size_t size() const { return N; }
	
	Vector() {}
	
	Vector(T s)
	{
		for (auto iter = _array.begin(); iter != _array.end(); ++iter)
			*iter = s;
	}
	
	Vector(std::array<T, N> &array) { _array = array; }
	
	template <size_t T_Index> T &e() { return _array[T_Index]; }
	template <size_t T_Index> const T &e() const { return _array[T_Index]; }
	
	template <size_t T_Index> Vector extract() const { return Vector(e<T_Index>()); }
	
	T &operator[](size_t index) { return _array[index]; }
	const T &operator[](size_t index) const { return  _array[index]; }
	
	template <typename Compare>
	Vector<bool, N> compare(const Vector &other, Compare cmp) const
	{
		std::array<bool, N> ret;
		
		auto iter = _array.begin();
		auto otherIter = other._array.begin();
		auto retIter = ret.begin();
		
		while (iter != _array.end())
			*retIter++ = cmp(*iter++, *otherIter++);
		
		return ret;
	}
	
	Vector<bool, N> operator<(const Vector &other) const { return compare(other, [](T a, T b){ return a<b; }); }
	Vector<bool, N> operator<=(const Vector &other) const { return compare(other, [](T a, T b){ return a<=b; }); }
	Vector<bool, N> operator>(const Vector &other) const { return compare(other, [](T a, T b){ return a>b; }); }
	Vector<bool, N> operator>=(const Vector &other) const { return compare(other, [](T a, T b){ return a>=b; }); }
	Vector<bool, N> operator==(const Vector &other) const { return compare(other, [](T a, T b){ return a==b; }); }
	Vector<bool, N> operator!=(const Vector &other) const { return compare(other, [](T a, T b){ return a!=b; }); }
	
private:
	
	std::array<T, N> _array;
};



}

#endif // VECTOR_GENERIC_H
