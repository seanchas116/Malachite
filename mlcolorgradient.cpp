#include "mlcolorgradient.h"

MLColorGradientCache::MLColorGradientCache(MLColorGradient *gradient, int sampleCount) :
	MLColorGradient(),
	_sampleCount(sampleCount),
	_cache(sampleCount + 1)
{
	for (int i = 0; i <= sampleCount; ++i)
	{
		_cache[i] = gradient->at(1.0 / sampleCount * i);
	}
}


MLVec4F MLArgbGradient::at(float x) const
{
	int count = _stops.size();
	if (count == 0)
		return MLVec4F(0);
	if (count == 1)
		return _stops.value(0);
	
	QMapIterator<float, MLArgb> i(_stops);
	
	if (x <= i.peekNext().key()) return i.peekNext().value();
	i.next();
	
	for (; i.hasNext(); i.next())
	{
		float x1 = i.peekNext().key();
		if (x < x1)
		{
			float x0 = i.peekPrevious().key();
			MLArgb y0 = i.peekPrevious().value();
			MLArgb y1 = i.peekNext().value();
			
			MLArgb r = y0 + (x - x0) / (x1 - x0) * (y1 - y0);
			return r;
		}
		if (x == x1)
			return i.peekNext().value();
	}
	
	return _stops.values().last();
}

