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

namespace Malachite
{

struct PaintEngineState
{
	BlendMode blendMode;
	Brush brush;
	double opacity;
	QTransform shapeTransform;
	Malachite::ImageTransformType imageTransformType;
};

class PaintEngine
{
public:
	PaintEngine();
	virtual ~PaintEngine() {}
	
	virtual bool begin(Paintable *paintable) = 0;
	virtual bool flush() = 0;
	
	virtual void drawTransformedPolygons(const FixedMultiPolygon &polygons) = 0;
	virtual void drawTransformedImage(const QPoint &point, const Image &image) = 0;
	virtual void drawTransformedSurface(const QPoint &point, const Surface &surface);
	
	virtual void drawTransformedPolygons(const MultiPolygon &polygons);
	virtual void drawPolygons(const MultiPolygon &polygons);
	virtual void drawPath(const QPainterPath &path);
	virtual void drawEllipse(double x, double y, double rx, double ry);
	virtual void drawRect(double x, double y, double width, double height);
	virtual void drawImage(const Vec2D &point, const Image &image);
	virtual void drawSurface(const Vec2D &point, const Surface &surface);
	
	PaintEngineState *state() { return &_state; }
	
	void pushState();
	void popState();
	
private:
	PaintEngineState _state;
	QStack<PaintEngineState> _stateStack;
};

}

#endif // MLPAINTENGINE_H
