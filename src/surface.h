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

namespace Malachite
{

typedef QHash<QPoint, Image *>	MLSurfaceHash;

class SurfaceData : public QSharedData
{
public:
	SurfaceData();
	SurfaceData(const SurfaceData &other);
	~SurfaceData() { qDeleteAll(tileHash); }
	
	MLSurfaceHash tileHash;
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
	
	bool save(QIODevice *device) const;
	static Surface loaded(QIODevice *device);
	
	bool isNull() const;
	
	Image tileForKey(const QPoint &key) const;
	Image tileForKey(int x, int y) const { return tileForKey(QPoint(x, y)); }
	
	Vec4F pixel(const QPoint &pos) const
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
	
	Surface section(const QPointSet &keys) const;
	Surface exclusion(const QPointSet &keys) const;
	
	template <bool DstInverted = false, bool SrcInverted = false, typename Image>
	void paste(const Image &image, const QPoint &point = QPoint());
	//template <typename Image> void paste(const Image &image, bool inverted = false, const QPoint &point = QPoint());
	//template <typename Image> void paste(const Image &image) { fromImage(QPoint(), image); }
	
	static Image DefaultTile;
	static Image WhiteTile;
	
private:
	int commonLeftBound(const QPointSet &keys) const;
	int commonRightBound(const QPointSet &keys) const;
	int commonTopBound(const QPointSet &keys) const;
	int commonBottomBound(const QPointSet &keys) const;
	
	void setupData() { if (!d) d = new SurfaceData; }
	
	QSharedDataPointer<SurfaceData> d;
};



class MALACHITESHARED_EXPORT SurfaceEditor
{
public:
	
	SurfaceEditor(Surface *surface) : _surface(surface) {}
	~SurfaceEditor();
	
	void deleteTile(const QPoint &key);
	void clear();
	Image *replaceTile(const QPoint &key, Image *image);
	Image *takeTile(const QPoint &key);
	
	void replace(const Surface &surface, const QPointSet &keys);
	
	QPointSet editedKeys() const { return _editedKeys; }
	
	Image *tileRefForKey(const QPoint &key);
	Image *tileRefForKey(int x, int y) { return tileRefForKey(QPoint(x, y)); }
	const Image *constTileRefForKey(const QPoint &key) const;
	const Image *constTileRefForKey(int x, int y) const { return constTileRefForKey(QPoint(x, y)); }
	const Image *tileRefForKey(const QPoint &key) const { return constTileRefForKey(key); }
	const Image *tileRefForKey(int x, int y) const { return constTileRefForKey(QPoint(x, y)); }
	
	const Surface *surface() { return _surface; }
	
private:
	
	void squeeze(const QPointSet &keys);
	
	Surface *_surface;
	QPointSet _editedKeys;
};


template <bool DstInverted, bool SrcInverted, typename Image>
void Surface::paste(const Image &image, const QPoint &point)
{
	SurfaceEditor editor(this);
	QPointSet keys = keysForRect(QRect(point, image.size()));
	
	foreach (const QPoint &key, keys)
		editor.tileRefForKey(key)->template paste<DstInverted, SrcInverted>(image, point - key * Surface::TileSize);
}

}

Q_DECLARE_METATYPE(Malachite::Surface)

#endif // MLSURFACE_H
