#ifndef MLPAINTABLE_H
#define MLPAINTABLE_H

#include "mlglobal.h"

namespace Malachite
{

class PaintEngine;

class MALACHITESHARED_EXPORT Paintable
{
public:
	virtual ~Paintable() {}
	virtual PaintEngine *createPaintEngine() = 0;
};

}

#endif // MLPAINTABLE_H
