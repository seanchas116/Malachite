#ifndef MLSURFACEPAINTENGINE_H
#define MLSURFACEPAINTENGINE_H

#include <QPaintEngine>
#include "mlsurface.h"
#include "mlpaintengine.h"

class MLSurfacePaintEngine : public MLPaintEngine
{
public:
	
	MLSurfacePaintEngine();
	~MLSurfacePaintEngine();
	
	bool begin(MLPaintable *paintable);
	bool flush();
	
	void drawEllipse(double x, double y, double rx, double ry);
	void drawPath(const QPainterPath &path);
	void drawImage(const QPoint &point, const MLImage &image);
	
	void setKeyClip(const QPointSet &keys) { _keyClip = keys; }
	QPointSet keyClip() const { return _keyClip; }
	
	QPointSet editedKeys() const { return _editor->editedKeys(); }
	
	void updateState(const MLPaintEngineState &state);
	
private:
	
	MLPaintEngineState _state;
	MLSurfaceEditor *_editor;
	QPointSet _keyClip;
};

#endif // MLSURFACEPAINTENGINE_H
