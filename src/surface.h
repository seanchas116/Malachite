#ifndef MLSURFACE_H
#define MLSURFACE_H

//ExportName: Surface

#include <QHash>
#include <QList>
#include <QSet>
#include <QSharedData>

#include "global.h"
#include "color.h"
#include "image.h"
#include "misc.h"
#include "image.h"
#include "division.h"
#include "genericsurface.h"

namespace Malachite
{

struct SurfaceDefaultTileProvider
{
	MALACHITESHARED_EXPORT static ImageU8 DefaultTileU8;
	MALACHITESHARED_EXPORT static Image DefaultTile;
	MALACHITESHARED_EXPORT static Image WhiteTile;
};

template <>
inline ImageU8 surfaceDefaultTile<ImageU8, 64>()
{
	return SurfaceDefaultTileProvider::DefaultTileU8;
}

typedef GenericSurface<ImageU8, 64> SurfaceU8;

template <>
inline Image surfaceDefaultTile<Image, 64>()
{
	return SurfaceDefaultTileProvider::DefaultTile;
}

class MALACHITESHARED_EXPORT Surface : public GenericSurface<Image, 64>, public Paintable
{
public:
	
	typedef GenericSurface<Image, 64> super;
	
	Surface() : super() {}
	Surface(const Surface &other) : super(other) {}
	
	PaintEngine *createPaintEngine() override;
};

class MALACHITESHARED_EXPORT SurfaceEditTracker
{
public:
	
	SurfaceEditTracker(Surface *surface) : _surface(surface) {}
	
	Image &tileRef(const QPoint &key)
	{
		_editedKeys << key;
		return _surface->tileRef(key);
	}
	
	QSet<QPoint> editedKeys() const
	{
		return _editedKeys;
	}
	
	Surface *surface() { return _surface; }
	const Surface *surface() const { return _surface; }
	
private:
	
	Surface *_surface;
	QSet<QPoint> _editedKeys;
};

MALACHITESHARED_EXPORT QDataStream &operator<<(QDataStream &out, const Surface &surface);
MALACHITESHARED_EXPORT QDataStream &operator>>(QDataStream &in, Surface &surfaceOut);

}

Q_DECLARE_METATYPE(Malachite::Surface)

#endif // MLSURFACE_H
