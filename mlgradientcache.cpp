#include <cmath>

#include "mlgradientcache.h"

MLGradientCache::MLGradientCache(const MLGradient *gradient)
{
	float *p = _cache;
	for (int i = 0; i <= CacheSize; ++i) {
		*p++ = gradient->evaluate((float)i / (float)CacheSize);
	}
}

float MLGradientCache::evaluate(float value) const
{
	// linear interpolation
	int index = floor(value * CacheSize);
	float offset = value * CacheSize - index;
	return _cache[index] * offset + _cache[index + 1] * (1.0f - offset);
}

