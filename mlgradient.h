#ifndef MLGRADIENT_H
#define MLGRADIENT_H

#include "mlglobal.h"

class MALACHITESHARED_EXPORT MLGradient
{
public:
    MLGradient() {}
    virtual ~MLGradient() {}
	
	virtual float evaluate(float value) const = 0;
};

#endif // MLGRADIENT_H
