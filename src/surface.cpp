#include <QtGui>

#include "misc.h"
#include "painter.h"
#include "private/surfacepaintengine.h"
#include "surface.h"
#include "division.h"

namespace Malachite
{

/*
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
		for (const QPoint &key : keys)
		{
			const Image image = tileForKey(key);
			for (int y = 0; y < Surface::TileSize; ++y)
			{
				if (image.pixel(x, y).a())
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
		for (const QPoint &key : keys)
		{
			const Image image = tileForKey(key);
			for (int y = 0; y < Surface::TileSize; ++y)
			{
				if (image.pixel(x, y).a())
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
		for (const QPoint &key : keys)
		{
			const Pixel *scanline = tileForKey(key).scanline(y);
			for (int x = 0; x < Surface::TileSize; ++x)
			{
				if (scanline->a())
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
		for (const QPoint &key : keys)
		{
			const Pixel *scanline = tileForKey(key).scanline(y);
			for (int x = 0; x < Surface::TileSize; ++x)
			{
				if (scanline->a())
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
*/

PaintEngine *Surface::createPaintEngine()
{
	return new SurfacePaintEngine();
}

Image SurfaceDefaultTileProvider::DefaultTile;
Image SurfaceDefaultTileProvider::WhiteTile;

class SurfaceDefaultTileInitializer
{
public:
	SurfaceDefaultTileInitializer()
	{
		SurfaceDefaultTileProvider::DefaultTile = Image(Surface::tileSize());
		SurfaceDefaultTileProvider::DefaultTile.fill(Color::fromRgbValue(0, 0, 0, 0).toPixel());
		SurfaceDefaultTileProvider::WhiteTile = Image(Surface::tileSize());
		SurfaceDefaultTileProvider::WhiteTile.fill(Color::fromRgbValue(1, 1, 1, 1).toPixel());
	}
};

static SurfaceDefaultTileInitializer defaultTileInitializer;

}

