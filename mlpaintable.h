#ifndef MLPAINTABLE_H
#define MLPAINTABLE_H

#include "mlglobal.h"

class MLPaintEngine;

class MALACHITESHARED_EXPORT MLPaintable
{
public:
	virtual ~MLPaintable() {}
	virtual MLPaintEngine *createPaintEngine() = 0;
};

#endif // MLPAINTABLE_H
