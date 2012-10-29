#ifndef MLSURFACEPAINTER_H
#define MLSURFACEPAINTER_H

//ExportName: SurfacePainter

#include "painter.h"

namespace Malachite
{

class MALACHITESHARED_EXPORT SurfacePainter : public Painter
{
public:
	SurfacePainter(Surface *surface) :
		Painter(surface) {}
	
	void setKeyClip(const QPointSet &keys);
	QPointSet keyClip() const;
	
	QPointSet editedKeys() const;
};

}

#endif // MLSURFACEPAINTER_H
