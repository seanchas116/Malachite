#ifndef MLPIXELGENERATOR_H
#define MLPIXELGENERATOR_H

#include "mlcolor.h"

class MLPixelGenerator
{
public:
	MLPixelGenerator() {}
	virtual ~MLPixelGenerator() {}
	
	virtual MLArgb at(const QPointF &point) = 0;
};

#endif // MLPIXELGENERATOR_H
