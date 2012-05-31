#ifndef MLPAINTER_H
#define MLPAINTER_H

#include "mlpaintengine.h"
#include "mlpaintable.h"
#include "mlsurface.h"
#include "mlbrush.h"

class MALACHITESHARED_EXPORT MLPainter
{
public:
	MLPainter(MLPaintable *paintable);
	~MLPainter();
	
	bool begin(MLPaintable *paintable);
	void flush();
	void end();
	
	void setBlendMode(const MLBlendMode &mode) { _state.blendMode = mode; _paintEngine->updateState(_state); }
	MLBlendMode blendMode() const { return _state.blendMode; }
	
	void setBrush(const MLBrush &brush) { _state.brush = brush; }
	MLBrush brush() const { return _state.brush; }
	
	void setColor(const MLColor &color) { setBrush(MLBrush(color)); }
	void setArgb(const MLFastArgbF &argb) { setBrush(MLBrush(argb)); }
	
	void setOpacity(double opacity) { _state.opacity = opacity; _paintEngine->updateState(_state); }
	double opacity() const { return _state.opacity; }
	
	void setState(const MLPaintEngineState &state) { _state = state; _paintEngine->updateState(state); }
	MLPaintEngineState state() { return _state; }
	
	void drawPath(const QPainterPath &path);
	void drawEllipse(const QRectF &rect);
	void drawEllipse(const QPointF &center, double rx, double ry);
	void drawImage(const QPoint &point, const MLImage &image);
	void drawImage(int x, int y, const MLImage &image) { drawImage(QPoint(x, y), image); }
	void drawSurface(const QPoint &point, const MLSurface &surface);
	void drawSurface(int x, int y, const MLSurface &surface) { drawSurface(QPoint(x, y), surface); }
	
protected:
	MLPaintEngine *paintEngine() { return _paintEngine; }
	const MLPaintEngine *paintEngine() const { return _paintEngine; }
	
private:
	MLPaintEngineState _state;
	MLPaintable *_paintable;
	MLPaintEngine *_paintEngine;
};

#endif // MLPAINTER_H
