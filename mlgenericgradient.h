#ifndef MLGENERICGRADIENT_H
#define MLGENERICGRADIENT_H

#include <QMap>
#include <QDebug>
#include <cmath>

template <class VarX, class VarY>
class MLGenericLineGradient
{
public:
	
	typedef VarX VarXType;
	typedef VarY VarYType;
	
	MLGenericLineGradient() {}
	
	void addStop(VarX x, VarY y)
	{
		_stops.insert(x, y);
	}
	
	void clear() { _stops.clear(); }
	
	VarY at(VarX x) const
	{
		int count = _stops.size();
		if (count == 0)
			return VarY();
		if (count == 1)
			return _stops.value(0);
		
		QMapIterator<VarX, VarY> i(_stops);
		
		if (x <= i.peekNext().key()) return i.peekNext().value();
		i.next();
		
		for (; i.hasNext(); i.next())
		{
			VarX x1 = i.peekNext().key();
			VarY y1 = i.peekNext().value();
			if (x < x1)
			{
				VarX x0 = i.peekPrevious().key();
				VarY y0 = i.peekPrevious().value();
				
				return y0 + (x - x0) / (x1 - x0) * (y1 - y0);
			}
			if (x == x1)
				return y1;
		}
		
		return _stops.values().at(0);
	}
	
private:
	QMap<VarX, VarY> _stops;
};

template <class VarX, class VarY>
class MLGenericGradientCache
{
public:
	
	typedef VarX VarXType;
	typedef VarY VarYType;
	
	MLGenericGradientCache(int size)
	{
		_size = size;
		_data = new VarY[size + 1];
	}
	
	MLGenericGradientCache(const MLGenericGradientCache &other)
	{
		_size = other._size;
		_data = new VarY[_size + 1];
		memcpy(_data, other._data, (_size + 1) * sizeof(VarY));
	}
	
	~MLGenericGradientCache()
	{
		delete [] _data;
	}
	
	template <class Gradient>
	void load(const Gradient &gradient)
	{
		typedef typename Gradient::VarXType	VarXOther;
		
		for (int i = 0; i <= _size; ++i)
		{
			_data[i] = gradient.at((VarXOther)i / (VarXOther)_size);
		}
	}
	
	VarY at(VarX x) const
	{
		Q_ASSERT(0 <= x && x <= 1);
		VarX f = floor(x);
		
		if (f == x)
			return _data[(int)f];
		
		VarX r = x - f;
		VarX d = 1 - r;
		return d * _data[(int)f] + r * _data[(int)f + 1];
	}
	
private:
	
	int _size;
	VarY *_data;
};


typedef MLGenericLineGradient<double, double> MLRealLineGradient;
typedef MLGenericGradientCache<double, double> MLRealGradientCache;

#endif // MLGENERICGRADIENT_H
