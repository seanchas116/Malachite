#pragma once

#include <QPoint>
#include <QHash>
#include <QSize>
#include <QRect>
#include "genericimage.h"
#include "division.h"
#include "list.h"

namespace Malachite
{

template <typename T_Image, int T_TileWidth>
inline T_Image surfaceDefaultTile()
{
	T_Image result(T_TileWidth, T_TileWidth);
	result.fill(0);
	return result;
}

template <typename T_Image, int T_TileWidth = 64>
class GenericSurface
{
public:
	
	typedef T_Image ImageType;
	typedef typename ImageType::PixelType PixelType;
	typedef QHash<QPoint, ImageType> HashType;
	typedef typename HashType::ConstIterator ConstIterator;
	typedef typename HashType::Iterator Iterator;
	
	typedef ConstIterator const_iterator;
	typedef Iterator iterator;
	
	GenericSurface() {}
	GenericSurface(const GenericSurface<ImageType, T_TileWidth> &other) : _hash(other._hash) {}
	
	constexpr static int tileWidth() { return T_TileWidth; }
	static QSize tileSize() { return QSize(tileWidth(), tileWidth()); }
	static QPoint tileSizePoint() { return QPoint(tileWidth(), tileWidth()); }
	
	bool isEmpty() const { return _hash.isEmpty(); }
	
	ImageType tile(const QPoint &key) const {return _hash.value(key, defaultTile()); }
	ImageType tile(int x, int y) const { return tile(QPoint(x, y)); }
	
	ImageType &tileRef(const QPoint &key)
	{
		if (!_hash.contains(key))
			_hash[key] = defaultTile();
		
		return _hash[key];
	}
	
	ImageType &tileRef(int x, int y) { return tileRef(QPoint(x, y)); }
	
	PixelType pixel(const QPoint &pos) const
	{
		QPoint key, rem;
		IntDivision::dividePoint(pos, tileWidth(), &key, &rem);
		return tile(key).pixel(rem);
	}
	
	bool contains(const QPoint &key) const { return _hash.contains(key); }
	bool contains(int x, int y) const { return contains(QPoint(x, y)); }
	
	int tileCount() const { return _hash.size(); }
	
	QSet<QPoint> keys() const { return _hash.keys().toSet(); }
	QList<QPoint> keyList() const { return _hash.keys(); }
	QSet<QPoint> keysInRect(const QRect &rect) const { return keys() & rectToKeys(rect); }
	
	void remove(const QPoint &key)
	{
		_hash.remove(key);
	}
	
	void clear()
	{
		_hash.clear();
	}
	
	void newTile(const QPoint &key)
	{
		_hash[key] = defaultTile();
	}
	
	template <ImagePasteInversionMode T_InversionMode = ImagePasteNotInverted, typename OtherImage>
	void paste(const OtherImage &image, const QPoint &pos = QPoint())
	{
		for (const QPoint &key : rectToKeys(QRect(pos, image.size())))
			tileRef(key).template paste<T_InversionMode>(image, pos - key * tileWidth());
	}
	
	template <typename OtherImage>
	OtherImage crop(const QRect &rect)
	{
		OtherImage image(rect.size());
		image.clear();
		auto keys = rectToKeys(rect);
		
		for (auto key : keys)
		{
			if (contains(key))
				image.paste(tile(key), -rect.topLeft() + key * tileWidth());
		}
		
		return image;
	}
	
	QRect boudingRect() const
	{
		List<QPoint> keys = _hash.keys();
		
		auto keyXs = keys.map( []( const QPoint &p ){ return p.x(); } );
		auto keyYs = keys.map( []( const QPoint &p ){ return p.y(); } );
		
		QPoint keyTopLeft( keyXs.min(), keyYs.min() ), keyBottomRight( keyXs.max(), keyYs.max() );
		
		auto keysWithX = [](const List<QPoint> &keys, int x)
		{
			return keys.select( [x](const QPoint &p){ return p.x() == x; } );
		};
		
		auto keysWithY = [](const List<QPoint> &keys, int y)
		{
			return keys.select( [y](const QPoint &p){ return p.y() == y; } );
		};
		
		QPoint topLeft, bottomRight;
		
		forever
		{
			if (keyTopLeft.x() > keyBottomRight.x() || keyTopLeft.y() > keyBottomRight.y())
				return QRect();
			
			auto leftKeys = keysWithX( keyTopLeft.x() );
			auto rightKeys = keysWithX( keyBottomRight.x() );
			
			auto topKeys = keysWithY( keyTopLeft.y() );
			auto bottomKeys = keysWithY( keyBottomRight.y() );
			
			topLeft.rx() = leftKeys.map( [](const QPoint &key){ return minOpaqueX(key); } ).min();
			topLeft.ry() = topKeys.map( [](const QPoint &key){ return minOpaqueY(key); } ).min();
			
			bottomRight.rx() = rightKeys.map( [](const QPoint &key){ return maxOpaqueX(key); } ).max();
			bottomRight.ry() = bottomKeys.map( [](const QPoint &key){ return maxOpaqueY(key); } ).max();
			
			if (topLeft.x() == tileWidth())
			{
				keyTopLeft.rx()++;
				continue;
			}
			
			if (topLeft.y() == tileWidth())
			{
				keyTopLeft.ry()++;
				continue;
			}
			
			if (bottomRight.x() == -1)
			{
				keyBottomRight.rx()--;
				continue;
			}
			
			if (bottomRight.y() == -1)
			{
				keyBottomRight.ry()--;
				continue;
			}
			
			break;
		}
		
		QRect rect;
		rect.setTopLeft(topLeft);
		rect.setBottomRight(bottomRight);
		
		return rect;
	}
	
	void squeeze(const QPointSet &keys)
	{
		for (const QPoint &key : keys)
		{
			if (_hash.contains(key))
			{
				if (_hash[key].isBlank())
					_hash.remove(key);
			}
		}
	}
	
	void squeeze()
	{
		List<QPoint> keyToRemove;
		
		for (auto iter = _hash.begin(); iter != _hash.end(); ++iter)
		{
			if (iter.value().isBlank())
				keyToRemove << iter.key();
		}
		
		for (auto key : keyToRemove)
			_hash.remove(key);
	}
	
	bool operator==(const GenericSurface &other)
	{
		return _hash == other._hash;
	}
	
	bool operator!=(const GenericSurface &other)
	{
		return !operator==(other);
	}
	
	static ImageType defaultTile()
	{
		return surfaceDefaultTile<ImageType, tileWidth()>();
	}
	
	static QPoint keyForPixel(const QPoint &pos)
	{
		QPoint key; 
		IntDivision::dividePoint(pos, tileWidth(), &key);
		return key;
	}
	
	static QRect keyToRect(const QPoint &pos)
	{
		return QRect(pos * tileWidth(), tileSize());
	}
	
	static QRect keyToRect(int x, int y) { return keyToRect(QPoint(x, y)); }
	
	static QSet<QPoint> rectToKeys(const QRect &rect)
	{
		QSet<QPoint> set;
		
		QPoint topLeftKey = keyForPixel(rect.topLeft());
		QPoint bottomRightKey = keyForPixel(rect.bottomRight());
		
		QRect keyRect(topLeftKey, bottomRightKey);
		set.reserve(keyRect.width() * keyRect.height());
		
		for (int y = topLeftKey.y(); y <= bottomRightKey.y(); ++y)
		{
			for (int x = topLeftKey.x(); x <= bottomRightKey.x(); ++x)
				set << QPoint(x, y);
		}
		
		return set;
	}
	
	static QHash<QPoint, QRect> divideRect(const QRect &rect)
	{
		QHash<QPoint, QRect> result;
		
		for (const QPoint &key : rectToKeys(rect))
			result.insert(key, QRect(QPoint(), tileSize()) & rect.translated(key * -tileWidth()));
		
		return result;
	}
	
private:
	
	static bool isHLineOpaque(const ImageType &tile, int y)
	{
		auto p = tile.constScanline(y);
		
		for (int x = 0; x < tileWidth(); ++x)
		{
			if (p->a())
				return true;
			++p;
		}
		
		return false;
	}
	
	static bool isVLineOpaque(const ImageType &tile, int x)
	{
		for (int y = 0; y < tileWidth(); ++y)
		{
			if (tile.pixel(x, y).a())
				return true;
		}
		
		return false;
	}
	
	static int minOpaqueY(const ImageType &tile)
	{
		for (int y = 0; y < tileWidth(); ++y)
		{
			if (isHLineOpaque(tile, y))
				return y;
		}
		return tileWidth();
	}
	
	static int maxOpaqueY(const ImageType &tile)
	{
		for (int y = tileWidth() - 1; y >= 0; --y)
		{
			if (isHLineOpaque(tile, y))
				return y;
		}
		return -1;
	}
	
	static int minOpaqueX(const ImageType &tile)
	{
		for (int x = 0; x < tileWidth(); ++x)
		{
			if (isVLineOpaque(tile, x))
				return x;
		}
		return tileWidth();
	}
	
	static int maxOpaqueX(const ImageType &tile)
	{
		for (int x = tileWidth() - 1; x >= 0; --x)
		{
			if (isVLineOpaque(tile, x))
				return x;
		}
		return -1;
	}
	
	
	QHash<QPoint, ImageType> _hash;
};

}
