#ifndef MLGENERICGRADIENT_H
#define MLGENERICGRADIENT_H

#include <QMap>
#include <QDebug>
#include <cmath>

template <class Key, class Value>
class MLGenericLineGradient
{
public:
	
	typedef Key KeyType;
	typedef Value ValueType;
	
	MLGenericLineGradient() {}
	
	void addStop(Key x, Value y)
	{
		_stops.insert(x, y);
	}
	
	void clear() { _stops.clear(); }
	
	Value at(Key x) const
	{
		int count = _stops.size();
		if (count == 0)
			return Value();
		if (count == 1)
			return _stops.value(0);
		
		QMapIterator<Key, Value> i(_stops);
		
		if (x <= i.peekNext().key()) return i.peekNext().value();
		i.next();
		
		for (; i.hasNext(); i.next())
		{
			Key x1 = i.peekNext().key();
			Value y1 = i.peekNext().value();
			if (x < x1)
			{
				Key x0 = i.peekPrevious().key();
				Value y0 = i.peekPrevious().value();
				
				return y0 + (x - x0) / (x1 - x0) * (y1 - y0);
			}
			if (x == x1)
				return y1;
		}
		
		return _stops.values().at(0);
	}
	
private:
	QMap<Key, Value> _stops;
};

template <class Key, class Value>
class MLGenericGradientCache
{
public:
	
	typedef Key KeyType;
	typedef Value ValueType;
	
	MLGenericGradientCache(int size)
	{
		_size = size;
		_data = new ValueType[size + 1];
	}
	
	MLGenericGradientCache(const MLGenericGradientCache &other)
	{
		_size = other._size;
		_data = new ValueType[_size + 1];
		memcpy(_data, other._data, (_size + 1) * sizeof(ValueType));
	}
	
	~MLGenericGradientCache()
	{
		delete [] _data;
	}
	
	int size() const { return _size; }
	
	template <class Gradient>
	void load(const Gradient &gradient)
	{
		typedef typename Gradient::KeyType	OtherKeyType;
		
		for (int i = 0; i <= _size; ++i)
		{
			_data[i] = gradient.at((OtherKeyType)i / (OtherKeyType)_size);
		}
	}
	
	Value at(Key x) const
	{
		Q_ASSERT(0 <= x && x <= 1);
		x *= (Key)_size;
		Key f = floorf(x);
		
		if (f == x)
			return _data[(int)f];
		
		Key r = x - f;
		return ((Key)1 - r) * _data[(int)f] + r * _data[(int)f + 1];
	}
	
	Value &operator[](int index)
	{
		return _data[index];
	}
	
	const Value &operator[](int index) const
	{
		return _data[index];
	}
	
private:
	
	int _size;
	Value *_data;
};


typedef MLGenericLineGradient<double, double> MLRealLineGradient;
typedef MLGenericGradientCache<double, double> MLRealGradientCache;

#endif // MLGENERICGRADIENT_H
