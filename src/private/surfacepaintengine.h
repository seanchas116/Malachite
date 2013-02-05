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
	void drawTransformedImage(const QPoint &point, const Image &image, const QRect &imageMaskRect);
	
	void drawTransformedSurface(const QPoint &point, const Surface &surface);
	
	void setKeyClip(const QPointSet &keys) { _keyClip = keys; }
	QPointSet keyClip() const { return _keyClip; }
	
	void setKeyRectClip(const QHash<QPoint, QRect> &keyRectClip) { _keyRectClip = keyRectClip; }
	QHash<QPoint, QRect> keyRectClip() const { return _keyRectClip; }
	
private:
	
	Surface *_surface = 0;
	QPointSet _keyClip;
	QHash<QPoint, QRect> _keyRectClip;
};

}

#endif // MLSURFACEPAINTENGINE_H
