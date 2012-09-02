#ifndef MLPAINTER_H
#define MLPAINTER_H

#include "mlpaintengine.h"
#include "mlpaintable.h"
#include "mlsurface.h"
#include "mlbrush.h"
#include "mlvector.h"

class MALACHITESHARED_EXPORT MLPainter
{
public:
	
	MLPainter(MLPaintable *paintable);
	virtual ~MLPainter();
	
	bool begin(MLPaintable *paintable);
	void flush();
	void end();
	
	void setBlendMode(int modeIndex) { state()->blendMode = MLBlendMode(modeIndex); }
	void setBlendMode(const MLBlendMode &mode) { state()->blendMode = mode; }
	MLBlendMode blendMode() const { return state()->blendMode; }
	
	void setBrush(const MLBrush &brush) { state()->brush = brush; }
	MLBrush brush() const { return state()->brush; }
	
	void setColor(const MLColor &color) { setBrush(MLBrush(color)); }
	void setArgb(const MLVec4F &argb) { setBrush(MLBrush(argb)); }
	
	void setOpacity(double opacity) { state()->opacity = opacity; }
	double opacity() const { return state()->opacity; }
	
	void setWorldTransform(const QTransform &transform) { state()->shapeTransform = transform.inverted(); }
	QTransform worldTransform() const { return state()->shapeTransform.inverted(); }
	
	void setShapeTransform(const QTransform &transform) { state()->shapeTransform = transform; }
	QTransform shapeTransform() const { return state()->shapeTransform; }
	
	void setImageTransformType(ML::ImageTransformType type) { state()->imageTransformType = type; }
	ML::ImageTransformType imageTransformType() const { return state()->imageTransformType; }
	
	MLPaintEngineState *state() { Q_ASSERT(_paintEngine); return _paintEngine->state(); }
	const MLPaintEngineState *state() const { Q_ASSERT(_paintEngine); return _paintEngine->state(); }
	
	void drawTransformedPolygons(const MLFixedMultiPolygon &polygons)
		{ Q_ASSERT(_paintEngine); _paintEngine->drawTransformedPolygons(polygons); }
	void drawTransformedPolygons(const MLMultiPolygon &polygons)
		{ Q_ASSERT(_paintEngine); _paintEngine->drawTransformedPolygons(polygons); }
	void drawTransformedImage(const QPoint &pos, const MLImage &image)
		{ Q_ASSERT(_paintEngine); _paintEngine->drawTransformedImage(pos, image); }
	void drawTransformedSurface(const QPoint &pos, const MLSurface &surface)
		{ Q_ASSERT(_paintEngine); _paintEngine->drawTransformedSurface(pos, surface); }
	
	void drawPolygons(const MLMultiPolygon &polygons)
		{ Q_ASSERT(_paintEngine); _paintEngine->drawPolygons(polygons); }
	void drawPath(const QPainterPath &path)
		{ Q_ASSERT(_paintEngine); _paintEngine->drawPath(path); }
	void drawEllipse(const QRectF &rect)
		{ drawEllipse(rect.x() + rect.width() * 0.5, rect.y() + rect.height() * 0.5, rect.width() * 0.5, rect.height() * 0.5); }
	void drawEllipse(double x, double y, double rx, double ry)
		{ Q_ASSERT(_paintEngine); _paintEngine->drawEllipse(x, y, rx, ry); }
	void drawEllipse(const QPointF &center, double rx, double ry)
		{ drawEllipse(center.x(), center.y(), rx, ry); }
	void drawEllipse(const MLVec2D &center, double rx, double ry)
		{ drawEllipse(center.x, center.y, rx, ry); }
	void drawRect(const QRectF &rect)
		{ drawRect(rect.x(), rect.y(), rect.width(), rect.height()); }
	void drawRect(double x, double y, double width, double height)
		{ Q_ASSERT(_paintEngine); _paintEngine->drawRect(x, y, width, height); }
	void drawImage(const MLVec2D &pos, const MLImage &image)
		{ Q_ASSERT(_paintEngine); _paintEngine->drawImage(pos, image); }
	void drawImage(double x, double y, const MLImage &image)
		{ drawImage(MLVec2D(x, y), image); }
	void drawSurface(const MLVec2D &pos, const MLSurface &surface)
		{ Q_ASSERT(_paintEngine); _paintEngine->drawSurface(pos, surface); }
	void drawSurface(double x, double y, const MLSurface &surface)
		{ drawSurface(MLVec2D(x, y), surface); }
	
	void pushState() { Q_ASSERT(_paintEngine); _paintEngine->pushState(); }
	void popState() { Q_ASSERT(_paintEngine); _paintEngine->popState(); }
	
	void translateShape(double dx, double dy) { state()->shapeTransform.translate(dx, dy); }
	void translateShape(const QPointF &d) { translateShape(d.x(), d.y()); }
	void rotateShape(double angle) { state()->shapeTransform.rotate(angle); }
	void scaleShape(double sx, double sy) { state()->shapeTransform.scale(sx, sy); }
	
protected:
	
	MLPaintEngine *paintEngine() { return _paintEngine.data(); }
	const MLPaintEngine *paintEngine() const { return _paintEngine.data(); }
	
private:
	
	MLPaintable *_paintable;
	QScopedPointer<MLPaintEngine> _paintEngine;
};

#endif // MLPAINTER_H
