#include "private/surfacepaintengine.h"

#include "surfacepainter.h"

namespace Malachite
{

void SurfacePainter::setKeyClip(const QPointSet &keys)
{
	static_cast<SurfacePaintEngine *>(paintEngine())->setKeyClip(keys);
}

QPointSet SurfacePainter::keyClip() const
{
	return static_cast<const SurfacePaintEngine *>(paintEngine())->keyClip();
}

QPointSet SurfacePainter::editedKeys() const
{
	return static_cast<const SurfacePaintEngine *>(paintEngine())->editedKeys();
}

}

