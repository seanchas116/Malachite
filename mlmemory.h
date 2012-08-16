#ifndef MLMEMORY_H
#define MLMEMORY_H

#include <QtGlobal>
#include "mlglobal.h"

inline void *mlAllocateAlignedMemory(size_t size, size_t alignment)
{
	void *p;
	posix_memalign(&p, alignment, size);
	return p;
}

inline void mlFreeAlignedMemory(void *p)
{
	free(p);
}

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

//#define ML_DEBUG_MEMORY

#ifdef ML_DEBUG_MEMORY
#warning ML_DEBUG_MEMORY enabled.
#endif

template <class T>
class MLPointer
{
public:
	
#ifdef ML_DEBUG_MEMORY
	
	MLPointer() :
		_start(0),
		_byteSize(0),
		_p(0)
	{}
	
	MLPointer(const void *rangeStart, int rangeByteSize, T *p) :
		_start(rangeStart),
		_byteSize(rangeByteSize),
		_p(p)
	{
		Q_ASSERT(rangeStart);
		Q_ASSERT(rangeCorrect());
	}
	
	MLPointer(T *start, int byteSize) :
		_start(start),
		_byteSize(byteSize),
		_p(start)
	{
		Q_ASSERT(start);
	}
	
	MLPointer(T &value) :
		_start(&value),
		_byteSize(sizeof(T)),
		_p(&value)
	{
		Q_ASSERT(&value);
	}
	
#else
	
	MLPointer() :
		_p(0)
	{}
	
	MLPointer(void *rangeStart, int rangeByteSize, T *p) :
		_p(p)
	{ Q_UNUSED(rangeStart); Q_UNUSED(rangeByteSize); }
	
	MLPointer(T *start, int byteSize) :
		_p(start)
	{ Q_UNUSED(byteSize); }
	
	MLPointer(T *p) :
		_p(p)
	{}
	
	MLPointer(T &value) :
		_p(&value)
	{}
	
#endif
	
	bool isNull() const { return !_p; }
	bool isValid() const { return _p; }
	
	MLPointer byteOffset(int offset) const
	{
#ifdef ML_DEBUG_MEMORY
		return MLPointer(_start, _byteSize, reinterpret_cast<T *>(reinterpret_cast<size_t>(_p) + offset));
#else
		return MLPointer(reinterpret_cast<T *>(reinterpret_cast<size_t>(_p) + offset));
#endif
	}
	
	void pasteArray(MLPointer<const T> otherPointer, int elemCount)
	{
		pasteByte(otherPointer, elemCount * sizeof(T));
	}
	
	void pasteByte(MLPointer<const T> otherPointer, int byteCount)
	{
#ifdef ML_DEBUG_MEMORY
		Q_ASSERT(isValid());
		Q_ASSERT(otherPointer.isValid());
		
		Q_ASSERT(remainingByteCount() <= byteCount);
		Q_ASSERT(otherPointer.remainingByteCount() <= byteCount);
#endif
		memcpy(_p, otherPointer, byteCount);
	}
	
	void fill(const T &value, int count)
	{
		MLPointer<T> p = *this;
		
		for (int i = 0; i < count; ++i)
		{
			*p++ = value;
		}
	}
	
	template <class S>
	MLPointer<S> reinterpret() const
	{
#ifdef ML_DEBUG_MEMORY
		return MLPointer<S>(_start, _byteSize, reinterpret_cast<S *>(_p));
#else
		return MLPointer<S>(reinterpret_cast<S *>(_p));
#endif
	}
	
	template <class S>
	MLPointer<S> constCast() const
	{
#ifdef ML_DEBUG_MEMORY
		return MLPointer<S>(_start, _byteSize, const_cast<S *>(_p));
#else
		return MLPointer<S>(const_cast<S *>(_p));
#endif
	}
	
	MLPointer operator+(int i) const
	{
#ifdef ML_DEBUG_MEMORY
		return MLPointer(_start, _byteSize, _p + i);
#else
		return MLPointer(_p + i);
#endif
	}
	MLPointer operator-(int i) const { return operator+(-i); }
	
	MLPointer &operator+=(int i) { _p += i; return *this; }
	MLPointer &operator-=(int i) { _p -= i; return *this; }
	
	MLPointer &operator++() { ++_p; return *this; }
	MLPointer operator++(int) { MLPointer p = *this; ++_p; return p; }
	MLPointer &operator--() { --_p; return *this; }
	MLPointer operator--(int) { MLPointer p = *this; --_p;; return p; }
	
	operator MLPointer<const T>() const
	{
#ifdef ML_DEBUG_MEMORY
		return MLPointer<const T>(_start, _byteSize, _p);
#else
		return MLPointer<const T>(_p);
#endif
	}
	
	operator T*() const
	{
#ifdef ML_DEBUG_MEMORY
		Q_ASSERT(rangeCorrect());
		Q_ASSERT(isValid());
#endif
		return _p;
	}
	
	T &operator*() const
	{
#ifdef ML_DEBUG_MEMORY
		Q_ASSERT(rangeCorrect());
		Q_ASSERT(isValid());
#endif
		return *_p;
	}
	
	T *operator->() const
	{
#ifdef ML_DEBUG_MEMORY
		Q_ASSERT(rangeCorrect());
		Q_ASSERT(isValid());
#endif
		return _p;
	}
	
	T &operator[](int index) const { return *(*this + index); }
	
#ifdef ML_DEBUG_MEMORY
	int remainingByteCount() const { return reinterpret_cast<size_t>(_start) + _byteSize - reinterpret_cast<size_t>(_p); }

	bool rangeCorrect() const
	{
		int index = reinterpret_cast<size_t>(_p) - reinterpret_cast<size_t>(_start);
		return 0 <= index && index <= _byteSize - (int)sizeof(T);
	}
#endif
	
private:
	
#ifdef ML_DEBUG_MEMORY
	const void * _start;
	int _byteSize;
#endif
	T *_p;
};



template <class T>
class MLArray
{
public:
	
	MLArray(int size) :
		_data(new T[size]),
		_size(size)
	{}
	
	~MLArray()
	{
		delete[] _data;
	}
	
	int size() const { return _size; }
	
	MLPointer<T> data() { return p(0); }
	MLPointer<const T> data() const { return p(0); }
	
	MLPointer<T> p(int index) { return MLPointer<T>(_data, _size * sizeof(T), _data + index); }
	MLPointer<const T> p(int index) const { return MLPointer<const T>(_data, _size * sizeof(T), _data + index); }
	
	T &operator[](int index) { Q_ASSERT(0 <= index && index < _size); return _data[index]; }
	const T &operator[](int index) const { Q_ASSERT(0 <= index && index < _size); return _data[index]; }
	
private:
	
	T *_data;
	int _size;
};


/*
template <typename T> inline bool mlCopyArray (
    T *dest,
    const T *src,
    int count
    )
{
	Q_CHECK_PTR(dest);
	Q_CHECK_PTR(src);
	void *p = memcpy(dest, src, count * sizeof(T));
	return p ? true : false;
}

template <typename T> inline void mlFillArray (
    T *dest,
    const T &data,
    int count
    )
{
	Q_CHECK_PTR(dest);
	Q_ASSERT(count >= 0);
	for (int i = 0; i < count; ++i) {
		*dest++ = data;
	}
}*/


#endif // MLMEMORY_H
