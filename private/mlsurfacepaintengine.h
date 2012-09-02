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
	
	void drawTransformedPolygons(const MLFixedMultiPolygon &polygons);
	void drawTransformedImage(const QPoint &point, const MLImage &image);
	
	void setKeyClip(const QPointSet &keys) { _keyClip = keys; }
	QPointSet keyClip() const { return _keyClip; }
	
	QPointSet editedKeys() const { return _editor->editedKeys(); }
	
private:
	
	MLSurfaceEditor *_editor;
	QPointSet _keyClip;
};

#endif // MLSURFACEPAINTENGINE_H
