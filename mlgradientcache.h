#ifndef MLGRADIENTCACHE_H
#define MLGRADIENTCACHE_H

#include "mlglobal.h"
#include "mlgradient.h"

class MALACHITESHARED_EXPORT MLGradientCache : public MLGradient
{
public:
	enum {
		CacheSize = 1024
	};
	
	MLGradientCache(const MLGradient *gradient);
	
	float evaluate(float value) const;
	
private:
	float _cache[CacheSize];
};

#endif // MLGRADIENTCACHE_H
