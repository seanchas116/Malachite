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

class MLSurfacePaintEngine;

class MALACHITESHARED_EXPORT MLSurface : public MLPaintable
{
public:
	friend class MLSurfacePaintEngine;
	
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
	
	MLSurface section(const QPointSet &keys) const;
	MLSurface exclusion(const QPointSet &keys) const;
	
	MLSurface &operator=(const MLSurface &surface);
	
	static QPointSet keysForRect(const QRect &rect);
	
	static MLImage DefaultTile;
	static MLImage WhiteTile;
	
private:
	int commonLeftBound(const QPointSet keys) const;
	int commonRightBound(const QPointSet keys) const;
	int commonTopBound(const QPointSet keys) const;
	int commonBottomBound(const QPointSet keys) const;
	
	void setupData();
	
	QSharedDataPointer<MLSurfaceData> d;
};

Q_DECLARE_METATYPE(MLSurface)

#endif // MLSURFACE_H
