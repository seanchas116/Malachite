#include <QtGui>

#include "misc.h"
#include "painter.h"
#include "private/surfacepaintengine.h"
#include "surface.h"
#include "division.h"

namespace Malachite
{

SurfaceData::SurfaceData()
{}

SurfaceData::SurfaceData(const SurfaceData &other)
    : QSharedData(other),
      tileHash(other.tileHash)
{
	SurfaceHash::iterator i;
	for (i = tileHash.begin(); i != tileHash.end(); ++i)
	{
		i.value() = new Image(*(i.value()));
	}
}



PaintEngine *Surface::createPaintEngine()
{
	return new SurfacePaintEngine();
}

bool Surface::save(QIODevice *device) const
{
	QDataStream outStream(device);
	
	outStream << (quint32)(TileSize);
	
	if (!d)
		return true;
	
	SurfaceHash::const_iterator i;
	for (i = d->tileHash.begin(); i != d->tileHash.end(); ++i)
	{
		const Image *tile = i.value();
		
		QByteArray tileArray;
		QDataStream tileStream(&tileArray, QIODevice::WriteOnly);
		
		for (int y = 0; y < TileSize; ++y) {
			const Vec4F *p = tile->constScanline(y);
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

Surface Surface::loaded(QIODevice *device)
{
	QDataStream inStream(device);
	
	quint32 tileSize;
	inStream >> tileSize;
	
	Surface surface;
	surface.setupData();
	
	while (!inStream.atEnd())
	{
		QPoint key;
		QByteArray tileArray;
		
		inStream >> key;
		inStream >> tileArray;
		
		Image *tile = new Image(TileSize, TileSize);
		QDataStream tileStream(&tileArray, QIODevice::ReadOnly);
		
		for (int y = 0; y < TileSize; ++y)
		{
			Vec4F *p = tile->scanline(y);
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
			Painter painter(&surface);
			painter.setBlendMode(BlendMode::Source);
			painter.drawTransformedImage(key * (int)tileSize, *tile);
		}
		delete tile;
	}
	
	return surface;
}

bool Surface::isNull() const
{
	if (!d || d->tileHash.isEmpty())
		return true;
	return false;
}

Image Surface::tileForKey(const QPoint &key) const
{
	if (!d)
		return Surface::DefaultTile;
	
	if (d->tileHash.contains(key))
		return *(d->tileHash[key]);
	else
		return Surface::DefaultTile;
}

QPointSet Surface::keysForRect(const QRect &rect)
{
	QPointSet set;
	
	QPoint topLeftKey = keyForPixel(rect.topLeft());
	QPoint bottomRightKey = keyForPixel(rect.bottomRight());
	
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

QHash<QPoint, QRect> Surface::divideRect(const QRect &rect)
{
	QHash<QPoint, QRect> result;
	
	for (const QPoint &key : keysForRect(rect))
		result.insert(key, QRect(0, 0, TileSize, TileSize) & rect.translated(key * -TileSize));
	
	return result;
}

QRect Surface::boundingKeyRect() const
{
	if (isNull())
		return QRect();
	
	SurfaceHash::const_iterator iterator = d->tileHash.begin();
	
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
QRect Surface::boundingRect() const
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

Surface Surface::section(const QPointSet &keys) const
{
	if (isNull())
		return Surface();
	
	Surface surface;
	
	foreach (const QPoint &key, keys)
	{
		if (!d->tileHash.contains(key))
			continue;
		surface.setupData();
		surface.d->tileHash.insert(key, new Image(*(d->tileHash.value(key))));
	}
	return surface;
}

Surface Surface::exclusion(const QPointSet &keys) const
{
	if (isNull())
		return Surface();
	
	Surface surface;
	
	foreach (const QPoint &key, d->tileHash.keys())
	{
		if (keys.contains(key))
			continue;
		surface.setupData();
		surface.d->tileHash.insert(key, new Image(*(d->tileHash.value(key))));
	}
	return surface;
}

void Surface::squeeze(const QPointSet &keys)
{
	if (isNull()) return;
	
	for (const QPoint &key : keys)
	{
		if (d->tileHash.contains(key))
		{
			if (d->tileHash.value(key)->isBlank())
			{
				delete d->tileHash.take(key);
			}
		}
	}
}

int Surface::commonLeftBound(const QPointSet &keys) const
{
	for (int x = 0; x < Surface::TileSize; ++x)
	{
		foreach (const QPoint &key, keys)
		{
			const Image image = tileForKey(key);
			for (int y = 0; y < Surface::TileSize; ++y)
			{
				if (image.pixel(x, y).a)
					return x;
			}
		}
	}
	return -1;
}

int Surface::commonRightBound(const QPointSet &keys) const
{
	for (int x = Surface::TileSize - 1; x >= 0; --x)
	{
		foreach (const QPoint &key, keys)
		{
			const Image image = tileForKey(key);
			for (int y = 0; y < Surface::TileSize; ++y)
			{
				if (image.pixel(x, y).a)
					return x;
			}
		}
	}
	return -1;
}

int Surface::commonTopBound(const QPointSet &keys) const
{
	for (int y = 0; y < Surface::TileSize; ++y)
	{
		foreach (const QPoint &key, keys)
		{
			const Vec4F *scanline = tileForKey(key).scanline(y);
			for (int x = 0; x < Surface::TileSize; ++x)
			{
				if (scanline->a)
					return y;
				scanline++;
			}
		}
	}
	return -1;
}

int Surface::commonBottomBound(const QPointSet &keys) const
{
	for (int y = Surface::TileSize - 1; y >= 0; --y)
	{
		foreach (const QPoint &key, keys)
		{
			const Vec4F *scanline = tileForKey(key).scanline(y);
			for (int x = 0; x < Surface::TileSize; ++x)
			{
				if (scanline->a)
					return y;
				scanline++;
			}
		}
	}
	return -1;
}

bool Surface::operator==(const Surface &other) const
{
	if (d == other.d)
		return true;
	
	if (!d || !other.d)
		return false;
	
	QPointSet keys = this->keys() & other.keys();
	
	for (auto key : keys)
	{
		if (this->tileForKey(key) != other.tileForKey(key))
			return false;
	}
	
	return true;
}

Image Surface::DefaultTile;
Image Surface::WhiteTile;

class MLSurfaceInitializer
{
public:
	MLSurfaceInitializer()
	{
		Surface::DefaultTile = Image(Surface::TileSize, Surface::TileSize);
		Surface::DefaultTile.fill(Color::fromRgbValue(0, 0, 0, 0).toArgb());
		Surface::WhiteTile = Image(Surface::TileSize, Surface::TileSize);
		Surface::WhiteTile.fill(Color::fromRgbValue(1, 1, 1, 1).toArgb());
	}
};

MLSurfaceInitializer fsSurfaceInitializer;




SurfaceEditor::~SurfaceEditor()
{
}

void SurfaceEditor::deleteTile(const QPoint &key)
{
	if (_surface->isNull()) return;
	
	Image *tile = takeTile(key);
	if (tile) delete tile;
}

void SurfaceEditor::clear()
{
	if (_surface->isNull()) return;
	
	_editedKeys += _surface->keys();
	qDeleteAll(_surface->d->tileHash);
	_surface->d->tileHash.clear();
}

Image *SurfaceEditor::takeTile(const QPoint &key)
{
	if (_surface->isNull()) return 0;
	if (!_surface->d->tileHash.contains(key)) return 0;
	
	_editedKeys << key;
	return _surface->d->tileHash.take(key);
}

Image *SurfaceEditor::replaceTile(const QPoint &key, Image *image)
{
	_editedKeys << key;
	Image *tile = takeTile(key);
	
	_surface->setupData();
	_surface->d->tileHash.insert(key, image);
	
	return tile;
}

void SurfaceEditor::replace(const Surface &surface, const QPointSet &keys)
{
	if (surface.isNull()) return;
	
	_surface->setupData();
	
	foreach (const QPoint &key, keys)
	{
		Image *oldTile = replaceTile(key, new Image(surface.tileForKey(key)));
		if (oldTile)
			delete oldTile;
	}
}

// Returns tile item with "key"
// If the tile hash no tile with "key", inserts a new tile and returns it
Image *SurfaceEditor::tileRefForKey(const QPoint &key)
{
	_surface->setupData();
	
	Image *tile;
	
	if (!_surface->d->tileHash.contains(key))
	{
		tile = new Image(Surface::DefaultTile);
		_surface->d->tileHash.insert(key, tile);
	}
	else
	{
		tile = _surface->d->tileHash[key];
	}
	_editedKeys << key;
	return tile;
}

const Image *SurfaceEditor::constTileRefForKey(const QPoint &key) const
{
	if (_surface->isNull() || _surface->d->tileHash.contains(key))
		return _surface->d->tileHash[key];
	else
		return &Surface::DefaultTile;
}

}

