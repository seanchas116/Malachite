#ifndef MLPAINTENGINE_H
#define MLPAINTENGINE_H

#include <QPainterPath>
#include <QStack>
#include "mlblendmode.h"
#include "mlpaintable.h"
#include "mlimage.h"
#include "mlsurface.h"
#include "mlbrush.h"
#include "mlfixedpolygon.h"

struct MLPaintEngineState
{
	MLBlendMode blendMode;
	MLBrush brush;
	double opacity;
	QTransform shapeTransform;
	ML::ImageTransformType imageTransformType;
};

class MLPaintEngine
{
public:
	MLPaintEngine();
	virtual ~MLPaintEngine() {}
	
	virtual bool begin(MLPaintable *paintable) = 0;
	virtual bool flush() = 0;
	
	virtual void drawTransformedPolygons(const MLFixedMultiPolygon &polygons) = 0;
	virtual void drawTransformedImage(const QPoint &point, const MLImage &image) = 0;
	virtual void drawTransformedSurface(const QPoint &point, const MLSurface &surface);
	
	virtual void drawTransformedPolygons(const MLMultiPolygon &polygons);
	virtual void drawPolygons(const MLMultiPolygon &polygons);
	virtual void drawPath(const QPainterPath &path);
	virtual void drawEllipse(double x, double y, double rx, double ry);
	virtual void drawRect(double x, double y, double width, double height);
	virtual void drawImage(const MLVec2D &point, const MLImage &image);
	virtual void drawSurface(const MLVec2D &point, const MLSurface &surface);
	
	MLPaintEngineState *state() { return &_state; }
	
	void pushState();
	void popState();
	
private:
	MLPaintEngineState _state;
	QStack<MLPaintEngineState> _stateStack;
};

#endif // MLPAINTENGINE_H
