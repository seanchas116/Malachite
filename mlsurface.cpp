#include <QtGui>

#include "mlmisc.h"
#include "mlpainter.h"
#include "private/mlsurfacepaintengine.h"
#include "mlsurface.h"
#include "mlintdivision.h"

MLSurfaceData::MLSurfaceData()
{}

MLSurfaceData::MLSurfaceData(const MLSurfaceData &other)
    : QSharedData(other),
      tileHash(other.tileHash)
{
	MLSurfaceHash::iterator i;
	for (i = tileHash.begin(); i != tileHash.end(); ++i)
	{
		i.value() = new MLImage(*(i.value()));
	}
}



MLPaintEngine *MLSurface::createPaintEngine()
{
	return new MLSurfacePaintEngine();
}

bool MLSurface::save(QIODevice *device) const
{
	QDataStream outStream(device);
	
	outStream << (quint32)(TileSize);
	
	if (!d)
		return true;
	
	MLSurfaceHash::const_iterator i;
	for (i = d->tileHash.begin(); i != d->tileHash.end(); ++i)
	{
		const MLImage *tile = i.value();
		
		QByteArray tileArray;
		QDataStream tileStream(&tileArray, QIODevice::WriteOnly);
		
		for (int y = 0; y < TileSize; ++y) {
			const MLVec4F *p = tile->constScanline(y);
			for (int x = 0; x < TileSize; ++x)
			{
				tileStream << p->a;
				tileStream << p->r;
				tileStream << p->g;
				tileStream << p->b;
				++p;
			}
		}
		
		outStream << i.key();
		outStream << tileArray;
	}
	return true;
}

MLSurface MLSurface::loaded(QIODevice *device)
{
	QDataStream inStream(device);
	
	quint32 tileSize;
	inStream >> tileSize;
	
	MLSurface surface;
	surface.setupData();
	
	while (!inStream.atEnd())
	{
		QPoint key;
		QByteArray tileArray;
		
		inStream >> key;
		inStream >> tileArray;
		
		MLImage *tile = new MLImage(TileSize, TileSize);
		QDataStream tileStream(&tileArray, QIODevice::ReadOnly);
		
		for (int y = 0; y < TileSize; ++y)
		{
			MLVec4F *p = tile->scanline(y);
			for (int x = 0; x < TileSize; ++x) 
			{
				tileStream >> p->a;
				tileStream >> p->r;
				tileStream >> p->g;
				tileStream >> p->b;
				++p;
			}
		}
		
		if (tileSize == TileSize)
		{
			if (!surface.d->tileHash.contains(key))
			{
				surface.d->tileHash[key] = tile;
				continue;
			}
		}
		else
		{
			MLPainter painter(&surface);
			painter.drawImage(key * (int)tileSize, *tile);
		}
		delete tile;
	}
	
	return surface;
}

bool MLSurface::isNull() const
{
	if (!d || d->tileHash.isEmpty())
		return true;
	return false;
}

MLImage MLSurface::tileForKey(const QPoint &key) const
{
	if (!d)
		return MLSurface::DefaultTile;
	
	if (d->tileHash.contains(key))
		return *(d->tileHash[key]);
	else
		return MLSurface::DefaultTile;
}

QPointSet MLSurface::keysForRect(const QRect &rect)
{
	QPointSet set;
	
	QPoint topLeftKey, bottomRightKey;
	MLIntDivision::dividePoint(rect.topLeft(), MLSurface::TileSize, &topLeftKey);
	MLIntDivision::dividePoint(rect.bottomRight(), MLSurface::TileSize, &bottomRightKey);
	
	QRect keyRect(topLeftKey, bottomRightKey);
	set.reserve(keyRect.width() * keyRect.height());
	
	for (int y = topLeftKey.y(); y <= bottomRightKey.y(); ++y)
	{
		for (int x = topLeftKey.x(); x <= bottomRightKey.x(); ++x)
		{
			set << QPoint(x, y);
		}
	}
	
	return set;
}

QRect MLSurface::boundingKeyRect() const
{
	if (isNull())
		return QRect();
	
	MLSurfaceHash::const_iterator iterator = d->tileHash.begin();
	
	int left, right, top, bottom;
	left = right = iterator.key().x();
	top = bottom = iterator.key().y();
	++iterator;
	
	for (; iterator != d->tileHash.end(); ++iterator)
	{
		int x = iterator.key().x();
		int y = iterator.key().y();
		
		if (left > x)
			left = x;
		if (right < x)
			right = x;
		if (top > y)
			top = y;
		if (bottom < y)
			bottom = y;
	}
	
	QRect rect;
	rect.setLeft(left);
	rect.setRight(right);
	rect.setTop(top);
	rect.setBottom(bottom);
	
	return rect;
}

// Returns the rectangle that bounds everything that is painted on the surface
QRect MLSurface::boundingRect() const
{
	if (isNull())
		return QRect();
	
	QRect keyRect = boundingKeyRect();
	
	QPointSet topTileKeys;
	for (int tileX = keyRect.left(); tileX <= keyRect.right(); ++tileX)
	{
		QPoint key(tileX, keyRect.top());
		if (!contains(key))
			continue;
		topTileKeys << key;
	}
	
	QPointSet bottomTileKeys;
	for (int tileX = keyRect.left(); tileX <= keyRect.right(); ++tileX)
	{
		QPoint key(tileX, keyRect.bottom());
		if (!contains(key))
			continue;
		bottomTileKeys << key;
	}
	
	QPointSet leftTileKeys;
	for (int tileY = keyRect.top(); tileY <= keyRect.bottom(); ++tileY)
	{
		QPoint key(keyRect.left(), tileY);
		if (!contains(key))
			continue;
		leftTileKeys << key;
	}
	
	QPointSet rightTileKeys;
	for (int tileY = keyRect.top(); tileY <= keyRect.bottom(); ++tileY)
	{
		QPoint key(keyRect.right(), tileY);
		if (!contains(key))
			continue;
		rightTileKeys << key;
	}
	
	QRect rect;
	rect.setTop(commonTopBound(topTileKeys));
	rect.setBottom(commonBottomBound(bottomTileKeys));
	rect.setLeft(commonLeftBound(leftTileKeys));
	rect.setRight(commonRightBound(rightTileKeys));
	
	return rect;
}

MLSurface MLSurface::section(const QPointSet &keys) const
{
	if (isNull())
		return MLSurface();
	
	MLSurface surface;
	
	foreach (const QPoint &key, keys)
	{
		if (!d->tileHash.contains(key))
			continue;
		surface.setupData();
		surface.d->tileHash.insert(key, new MLImage(*(d->tileHash.value(key))));
	}
	return surface;
}

MLSurface MLSurface::exclusion(const QPointSet &keys) const
{
	if (isNull())
		return MLSurface();
	
	MLSurface surface;
	
	foreach (const QPoint &key, d->tileHash.keys())
	{
		if (keys.contains(key))
			continue;
		surface.setupData();
		surface.d->tileHash.insert(key, new MLImage(*(d->tileHash.value(key))));
	}
	return surface;
}

int MLSurface::commonLeftBound(const QPointSet &keys) const
{
	for (int x = 0; x < MLSurface::TileSize; ++x)
	{
		foreach (const QPoint &key, keys)
		{
			const MLImage image = tileForKey(key);
			for (int y = 0; y < MLSurface::TileSize; ++y)
			{
				if (image.pixel(x, y).a)
					return x;
			}
		}
	}
	return -1;
}

int MLSurface::commonRightBound(const QPointSet &keys) const
{
	for (int x = MLSurface::TileSize - 1; x >= 0; --x)
	{
		foreach (const QPoint &key, keys)
		{
			const MLImage image = tileForKey(key);
			for (int y = 0; y < MLSurface::TileSize; ++y)
			{
				if (image.pixel(x, y).a)
					return x;
			}
		}
	}
	return -1;
}

int MLSurface::commonTopBound(const QPointSet &keys) const
{
	for (int y = 0; y < MLSurface::TileSize; ++y)
	{
		foreach (const QPoint &key, keys)
		{
			const MLVec4F *scanline = tileForKey(key).scanline(y);
			for (int x = 0; x < MLSurface::TileSize; ++x)
			{
				if (scanline->a)
					return y;
				scanline++;
			}
		}
	}
	return -1;
}

int MLSurface::commonBottomBound(const QPointSet &keys) const
{
	for (int y = MLSurface::TileSize - 1; y >= 0; --y)
	{
		foreach (const QPoint &key, keys)
		{
			const MLVec4F *scanline = tileForKey(key).scanline(y);
			for (int x = 0; x < MLSurface::TileSize; ++x)
			{
				if (scanline->a)
					return y;
				scanline++;
			}
		}
	}
	return -1;
}

MLImage MLSurface::DefaultTile;
MLImage MLSurface::WhiteTile;

class MLSurfaceInitializer
{
public:
	MLSurfaceInitializer()
	{
		MLSurface::DefaultTile = MLImage(MLSurface::TileSize, MLSurface::TileSize);
		MLSurface::DefaultTile.fill(MLColor::fromRgbValue(0, 0, 0, 0).toArgb());
		MLSurface::WhiteTile = MLImage(MLSurface::TileSize, MLSurface::TileSize);
		MLSurface::WhiteTile.fill(MLColor::fromRgbValue(1, 1, 1, 1).toArgb());
	}
};

MLSurfaceInitializer fsSurfaceInitializer;




MLSurfaceEditor::~MLSurfaceEditor()
{
	squeeze(_editedKeys);
}

void MLSurfaceEditor::deleteTile(const QPoint &key)
{
	if (_surface->isNull()) return;
	
	MLImage *tile = takeTile(key);
	if (tile) delete tile;
}

void MLSurfaceEditor::clear()
{
	if (_surface->isNull()) return;
	
	_editedKeys += _surface->keys();
	qDeleteAll(_surface->d->tileHash);
	_surface->d->tileHash.clear();
}

MLImage *MLSurfaceEditor::takeTile(const QPoint &key)
{
	if (_surface->isNull()) return 0;
	if (!_surface->d->tileHash.contains(key)) return 0;
	
	_editedKeys << key;
	return _surface->d->tileHash.take(key);
}

MLImage *MLSurfaceEditor::replaceTile(const QPoint &key, MLImage *image)
{
	_editedKeys << key;
	MLImage *tile = takeTile(key);
	
	_surface->setupData();
	_surface->d->tileHash.insert(key, image);
	
	return tile;
}

void MLSurfaceEditor::replace(const MLSurface &surface, const QPointSet &keys)
{
	if (surface.isNull()) return;
	
	_surface->setupData();
	
	foreach (const QPoint &key, keys)
	{
		MLImage *oldTile = replaceTile(key, new MLImage(surface.tileForKey(key)));
		if (oldTile)
			delete oldTile;
	}
}

// Returns tile item with "key"
// If the tile hash no tile with "key", inserts a new tile and returns it
MLImage *MLSurfaceEditor::tileRefForKey(const QPoint &key)
{
	_surface->setupData();
	
	MLImage *tile;
	
	if (!_surface->d->tileHash.contains(key))
	{
		tile = new MLImage(MLSurface::DefaultTile);
		_surface->d->tileHash.insert(key, tile);
	}
	else
	{
		tile = _surface->d->tileHash[key];
	}
	_editedKeys << key;
	return tile;
}

const MLImage *MLSurfaceEditor::constTileRefForKey(const QPoint &key) const
{
	if (_surface->isNull() || _surface->d->tileHash.contains(key))
		return _surface->d->tileHash[key];
	else
		return &MLSurface::DefaultTile;
}

void MLSurfaceEditor::squeeze(const QPointSet &keys)
{
	if (_surface->isNull()) return;
	
	QPointList deleteList;
	
	foreach (const QPoint &key, keys)
	{
		if (_surface->d->tileHash.contains(key))
		{
			if (_surface->d->tileHash.value(key)->isBlank())
				deleteList << key;
		}
	}
	
	foreach (const QPoint &key, deleteList)
	{
		delete _surface->d->tileHash.take(key);
	}
}


