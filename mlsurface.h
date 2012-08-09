#ifndef MLSURFACE_H
#define MLSURFACE_H

#include <QHash>
#include <QList>
#include <QSet>
#include <QSharedData>

#include "mlcolor.h"
#include "mlimage.h"
#include "mlmisc.h"
#include "mlimage.h"

typedef QHash<QPoint, MLImage *>	MLSurfaceHash;

class MLSurfaceData : public QSharedData
{
public:
	MLSurfaceData();
	MLSurfaceData(const MLSurfaceData &other);
	~MLSurfaceData() { qDeleteAll(tileHash); }
	
	MLSurfaceHash tileHash;
};

class MLSurfaceEditor;

class MALACHITESHARED_EXPORT MLSurface : public MLPaintable
{
public:
	friend class MLSurfaceEditor;
	
	enum {
		TileSize = 64
	};
	
	MLSurface() : MLPaintable() { d = 0; }
	MLSurface(const MLSurface &other) : MLPaintable(), d(other.d) {}
	
	MLPaintEngine *createPaintEngine();
	
	bool save(QIODevice *device) const;
	static MLSurface loaded(QIODevice *device);
	
	bool isNull() const;
	
	MLImage tileForKey(const QPoint &key) const;
	MLImage tileForKey(int x, int y) const { return tileForKey(QPoint(x, y)); }
	
	bool contains(const QPoint &key) const { return d ? d->tileHash.contains(key) : false; }
	bool contains(int x, int y) const { return contains(QPoint(x, y)); }
	
	int tileCount() const { return d ? d->tileHash.size() : 0; }
	
	QPointSet keys() const { return d ? d->tileHash.keys().toSet() : QPointSet(); }
	QPointSet keys(const QRect &rect) const { return keys() & keysForRect(rect); }
	
	QRect boundingKeyRect() const;
	QRect boundingRect() const;
	
	static QRect keyToRect(int x, int y) { return QRect(TileSize * x, TileSize * y, TileSize, TileSize); }
	static QRect keyToRect(const QPoint &point) { return keyToRect(point.x(), point.y()); }
	
	MLSurface section(const QPointSet &keys) const;
	MLSurface exclusion(const QPointSet &keys) const;
	
	template <typename Image> void paste(const Image &image, bool inverted = false, const QPoint &point = QPoint());
	//template <typename Image> void paste(const Image &image) { fromImage(QPoint(), image); }
	
	static QPointSet keysForRect(const QRect &rect);
	
	static MLImage DefaultTile;
	static MLImage WhiteTile;
	
private:
	int commonLeftBound(const QPointSet &keys) const;
	int commonRightBound(const QPointSet &keys) const;
	int commonTopBound(const QPointSet &keys) const;
	int commonBottomBound(const QPointSet &keys) const;
	
	void setupData() { if (!d) d = new MLSurfaceData; }
	
	QSharedDataPointer<MLSurfaceData> d;
};


Q_DECLARE_METATYPE(MLSurface)



class MLSurfaceEditor
{
public:
	
	MLSurfaceEditor(MLSurface *surface) : _surface(surface) {}
	~MLSurfaceEditor();
	
	void deleteTile(const QPoint &key);
	void clear();
	MLImage *replaceTile(const QPoint &key, MLImage *image);
	MLImage *takeTile(const QPoint &key);
	
	void replace(const MLSurface &surface, const QPointSet &keys);
	
	QPointSet editedKeys() const { return _editedKeys; }
	
	MLImage *tileRefForKey(const QPoint &key);
	MLImage *tileRefForKey(int x, int y) { return tileRefForKey(QPoint(x, y)); }
	const MLImage *constTileRefForKey(const QPoint &key) const;
	const MLImage *constTileRefForKey(int x, int y) const { return constTileRefForKey(QPoint(x, y)); }
	const MLImage *tileRefForKey(const QPoint &key) const { return constTileRefForKey(key); }
	const MLImage *tileRefForKey(int x, int y) const { return constTileRefForKey(QPoint(x, y)); }
	
private:
	
	void squeeze(const QPointSet &keys);
	
	MLSurface *_surface;
	QPointSet _editedKeys;
};


template <typename Image>
void MLSurface::paste(const Image &image, bool inverted, const QPoint &point)
{
	MLSurfaceEditor editor(this);
	QPointSet keys = keysForRect(QRect(point, image.size()));
	
	foreach (const QPoint &key, keys)
		editor.tileRefForKey(key)->paste(image, inverted, point - key * MLSurface::TileSize);
}

#endif // MLSURFACE_H
