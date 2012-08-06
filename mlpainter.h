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
	~MLPainter();
	
	bool begin(MLPaintable *paintable);
	void flush();
	void end();
	
	void setBlendMode(const MLBlendMode &mode) { _state.blendMode = mode; updateState(); }
	MLBlendMode blendMode() const { return _state.blendMode; }
	
	void setBrush(const MLBrush &brush) { _state.brush = brush; updateState(); }
	MLBrush brush() const { return _state.brush; }
	
	void setColor(const MLColor &color) { setBrush(MLBrush(color)); updateState(); }
	void setArgb(const MLArgb &argb) { setBrush(MLBrush(argb)); updateState(); }
	
	void setOpacity(double opacity) { _state.opacity = opacity; updateState(); }
	double opacity() const { return _state.opacity; }
	
	void setWorldTransform(const QTransform &transform) { _state.shapeTransform = transform.inverted(); updateState(); }
	QTransform worldTransform() const { return _state.shapeTransform.inverted(); }
	
	void setShapeTransform(const QTransform &transform) { _state.shapeTransform = transform; updateState(); }
	QTransform shapeTransform() const { return _state.shapeTransform; }
	
	void setImageTransformType(MLGlobal::ImageTransformType type) { _state.imageTransformType = type; updateState(); }
	MLGlobal::ImageTransformType imageTransformType() const { return _state.imageTransformType; }
	
	void setState(const MLPaintEngineState &state) { _state = state; updateState(); }
	MLPaintEngineState state() { return _state; }
	
	void drawPath(const QPainterPath &path);
	void drawEllipse(const QRectF &rect);
	void drawEllipse(double x, double y, double rx, double ry);
	void drawEllipse(const QPointF &center, double rx, double ry) { drawEllipse(center.x(), center.y(), rx, ry); }
	void drawEllipse(const MLVec2D &center, double rx, double ry) { drawEllipse(center.x, center.y, rx, ry); }
	void drawRect(const QRectF &rect);
	void drawRect(double x, double y, double width, double height) { drawRect(QRectF(x, y, width, height)); }
	void drawImage(const QPoint &point, const MLImage &image);
	void drawImage(int x, int y, const MLImage &image) { drawImage(QPoint(x, y), image); }
	void drawSurface(const QPoint &point, const MLSurface &surface);
	void drawSurface(int x, int y, const MLSurface &surface) { drawSurface(QPoint(x, y), surface); }
	
	void translateShape(double dx, double dy) { _state.shapeTransform.translate(dx, dy); updateState(); }
	void translateShape(const QPointF &d) { translateShape(d.x(), d.y()); }
	void rotateShape(double angle) { _state.shapeTransform.rotate(angle); updateState(); }
	void scaleShape(double sx, double sy) { _state.shapeTransform.scale(sx, sy), updateState(); }
	
protected:
	
	MLPaintEngine *paintEngine() { return _paintEngine.data(); }
	const MLPaintEngine *paintEngine() const { return _paintEngine.data(); }
	
private:
	
	void updateState() { _paintEngine->updateState(_state); }
	
	MLPaintEngineState _state;
	MLPaintable *_paintable;
	QScopedPointer<MLPaintEngine> _paintEngine;
};

#endif // MLPAINTER_H
