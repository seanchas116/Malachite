#ifndef MLSURFACEPAINTER_H
#define MLSURFACEPAINTER_H

#include "mlpainter.h"
#include "mlsurfacepaintengine.h"

class MALACHITESHARED_EXPORT MLSurfacePainter : public MLPainter
{
public:
	MLSurfacePainter(MLSurface *surface) :
		MLPainter(surface) {}
	
	void setKeyClip(const QPointSet &keys) { surfacePaintEngine()->setKeyClip(keys); }
	QPointSet keyClip() const { return surfacePaintEngine()->keyClip(); }
	
	QPointSet editedKeys() const { return surfacePaintEngine()->editedKeys(); }
	
	MLSurfacePaintEngine *surfacePaintEngine() { return static_cast<MLSurfacePaintEngine *>(paintEngine()); }
	const MLSurfacePaintEngine *surfacePaintEngine() const { return static_cast<const MLSurfacePaintEngine *>(paintEngine()); }
};

#endif // MLSURFACEPAINTER_H
