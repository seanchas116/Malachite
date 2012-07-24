#include "private/mlsurfacepaintengine.h"

#include "mlsurfacepainter.h"

void MLSurfacePainter::setKeyClip(const QPointSet &keys)
{
	static_cast<MLSurfacePaintEngine *>(paintEngine())->setKeyClip(keys);
}

QPointSet MLSurfacePainter::keyClip() const
{
	return static_cast<const MLSurfacePaintEngine *>(paintEngine())->keyClip();
}

QPointSet MLSurfacePainter::editedKeys() const
{
	return static_cast<const MLSurfacePaintEngine *>(paintEngine())->editedKeys();
}

