#ifndef MLPAINTENGINE_H
#define MLPAINTENGINE_H

#include <QPainterPath>
#include "mlblendmode.h"
#include "mlpaintable.h"
#include "mlimage.h"
#include "mlsurface.h"
#include "mlbrush.h"

struct MLPaintEngineState
{
	MLBlendMode blendMode;
	MLBrush brush;
	double opacity;
	QTransform worldTransform;
	MLGlobal::ImageTransformType imageTransformType;
};

class MLPaintEngine
{
public:
	MLPaintEngine() {}
	virtual ~MLPaintEngine() {}
	
	virtual bool begin(MLPaintable *paintable) = 0;
	virtual bool flush() = 0;
	
	virtual void drawPath(const QPainterPath &path);
	virtual void drawEllipse(double x, double y, double rx, double ry);
	virtual void drawRect(double x, double y, double width, double height);
	virtual void drawImage(const QPoint &point, const MLImage &image);
	virtual void drawSurface(const QPoint &point, const MLSurface &surface);
	
	virtual void updateState(const MLPaintEngineState &state) = 0;
};

#endif // MLPAINTENGINE_H
