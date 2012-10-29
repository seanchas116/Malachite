#ifndef MLSURFACEPAINTENGINE_H
#define MLSURFACEPAINTENGINE_H

#include <QPaintEngine>
#include "../surface.h"
#include "../paintengine.h"

namespace Malachite
{

class SurfacePaintEngine : public PaintEngine
{
public:
	
	SurfacePaintEngine();
	~SurfacePaintEngine();
	
	bool begin(Paintable *paintable);
	bool flush();
	
	void drawTransformedPolygons(const FixedMultiPolygon &polygons);
	void drawTransformedImage(const QPoint &point, const Image &image);
	void drawTransformedSurface(const QPoint &point, const Surface &surface);
	
	void setKeyClip(const QPointSet &keys) { _keyClip = keys; }
	QPointSet keyClip() const { return _keyClip; }
	
	QPointSet editedKeys() const { return _editor->editedKeys(); }
	
private:
	
	SurfaceEditor *_editor;
	QPointSet _keyClip;
};

}

#endif // MLSURFACEPAINTENGINE_H
