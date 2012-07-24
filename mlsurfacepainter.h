#ifndef MLSURFACEPAINTER_H
#define MLSURFACEPAINTER_H

#include "mlpainter.h"

class MALACHITESHARED_EXPORT MLSurfacePainter : public MLPainter
{
public:
	MLSurfacePainter(MLSurface *surface) :
		MLPainter(surface) {}
	
	void setKeyClip(const QPointSet &keys);
	QPointSet keyClip() const;
	
	QPointSet editedKeys() const;
};

#endif // MLSURFACEPAINTER_H
