#ifndef MLSURFACE_H
#define MLSURFACE_H

//ExportName: Surface

#include <QHash>
#include <QList>
#include <QSet>
#include <QSharedData>

#include "color.h"
#include "image.h"
#include "misc.h"
#include "image.h"
#include "division.h"
#include "genericsurface.h"

namespace Malachite
{

/*
typedef QHash<QPoint, Image *>	SurfaceHash;

class SurfaceData : public QSharedData
{
public:
	SurfaceData();
	SurfaceData(const SurfaceData &other);
	~SurfaceData() { qDeleteAll(tileHash); }
	
	SurfaceHash tileHash;
};

class SurfaceEditor;

class MALACHITESHARED_EXPORT Surface : public Paintable
{
public:
	friend class SurfaceEditor;
	
	enum {
		TileSize = 64
	};
	
	Surface() : Paintable() { d = 0; }
	Surface(const Surface &other) : Paintable(), d(other.d) {}
	
	PaintEngine *createPaintEngine();
	
	bool isNull() const;
	
	Image tileForKey(const QPoint &key) const;
	Image tileForKey(int x, int y) const { return tileForKey(QPoint(x, y)); }
	
	Pixel pixel(const QPoint &pos) const
	{
		QPoint key, rem;
		dividePoint(pos, TileSize, &key, &rem);
		return tileForKey(key).pixel(rem);
	}
	
	QSize size() const { return QSize(); }	// no size
	int width() const { return size().width(); }
	int height() const { return size().height(); }
	
	bool contains(const QPoint &key) const { return d ? d->tileHash.contains(key) : false; }
	bool contains(int x, int y) const { return contains(QPoint(x, y)); }
	
	int tileCount() const { return d ? d->tileHash.size() : 0; }
	
	QPointSet keys() const { return d ? d->tileHash.keys().toSet() : QPointSet(); }
	QPointSet keys(const QRect &rect) const { return keys() & keysForRect(rect); }
	
	QRect boundingKeyRect() const;
	QRect boundingRect() const;
	
	static QPoint keyForPixel(const QPoint &pos) { QPoint key; dividePoint(pos, TileSize, &key); return key; }
	
	static QRect keyToRect(int x, int y) { return QRect(TileSize * x, TileSize * y, TileSize, TileSize); }
	static QRect keyToRect(const QPoint &point) { return keyToRect(point.x(), point.y()); }
	
	static QPointSet keysForRect(const QRect &rect);
	
	static QHash<QPoint, QRect> divideRect(const QRect &rect);
	
	Surface section(const QPointSet &keys) const;
	Surface exclusion(const QPointSet &keys) const;
	
	void squeeze(const QPointSet &keys);
	
	template <ImagePasteInversionMode InversionMode = ImagePasteNotInverted, typename Image>
	void paste(const Image &image, const QPoint &point = QPoint());
	//template <typename Image> void paste(const Image &image, bool inverted = false, const QPoint &point = QPoint());
	//template <typename Image> void paste(const Image &image) { fromImage(QPoint(), image); }
	
	bool operator==(const Surface &other) const;
	
	static Image DefaultTile;
	static Image WhiteTile;
	
private:
	int commonLeftBound(const QPointSet &keys) const;
	int commonRightBound(const QPointSet &keys) const;
	int commonTopBound(const QPointSet &keys) const;
	int commonBottomBound(const QPointSet &keys) const;
	
	void setupData() { if (!d) d = new SurfaceData; }
	
	QSharedDataPointer<SurfaceData> d;
};*/

struct SurfaceDefaultTileProvider
{
	static Image DefaultTile;
	static Image WhiteTile;
};

template <>
inline Image surfaceDefaultTile<Image, 64>()
{
	return SurfaceDefaultTileProvider::DefaultTile;
}

class Surface : public GenericSurface<Image, 64>, public Paintable
{
public:
	
	typedef GenericSurface<Image, 64> super;
	
	Surface() : super() {}
	Surface(const Surface &other) : super(other) {}
	
	PaintEngine *createPaintEngine() override;
};

class SurfaceEditTracker
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


}

Q_DECLARE_METATYPE(Malachite::Surface)

#endif // MLSURFACE_H
