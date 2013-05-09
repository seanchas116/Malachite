
#include "misc.h"
#include "painter.h"
#include "private/surfacepaintengine.h"
#include "surface.h"
#include "division.h"

namespace Malachite
{

PaintEngine *Surface::createPaintEngine()
{
	return new SurfacePaintEngine();
}

QDataStream &operator<<(QDataStream &out, const Surface &surface)
{
    out << quint64(surface.tileWidth());
    out << quint64(surface.tileCount());
	
	for (auto iter = surface.begin(); iter != surface.end(); ++iter)
	{
        quint64 x, y;
		x = iter.key().x();
		y = iter.key().y();
		out << x;
		out << y;
		out << iter.value();
	}
	
	return out;
}

QDataStream &operator>>(QDataStream &in, Surface &surfaceOut)
{
    quint64 tileWidth2x, tileCount2x;
	in >> tileWidth2x;
	in >> tileCount2x;
	
	int tileWidth = tileWidth2x;
	int tileCount = tileCount2x;
	
	Surface surface;
	
	for (int i = 0; i < tileCount; ++i)
	{
        quint64 x, y;
		in >> x;
		in >> y;
		
		QPoint key(x, y);
		
		Image image;
		in >> image;
		
		if (image.isValid() && image.size() == QSize(tileWidth, tileWidth))
		{
			if (tileWidth == Surface::tileWidth())
				surface.setTile(key, image);
			else
				surface.paste(image, key * tileWidth);
		}
	}
	
	surfaceOut = surface;
	
	return in;
}

ImageU8 SurfaceDefaultTileProvider::DefaultTileU8;
Image SurfaceDefaultTileProvider::DefaultTile;
Image SurfaceDefaultTileProvider::WhiteTile;

class SurfaceDefaultTileInitializer
{
public:
	SurfaceDefaultTileInitializer()
	{
		SurfaceDefaultTileProvider::DefaultTileU8 = ImageU8(Surface::tileSize());
		SurfaceDefaultTileProvider::DefaultTileU8.fill(0);
		SurfaceDefaultTileProvider::DefaultTile = Image(Surface::tileSize());
		SurfaceDefaultTileProvider::DefaultTile.fill(0);
		SurfaceDefaultTileProvider::WhiteTile = Image(Surface::tileSize());
		SurfaceDefaultTileProvider::WhiteTile.fill(Color::fromRgbValue(1, 1, 1, 1).toPixel());
	}
};

static SurfaceDefaultTileInitializer defaultTileInitializer;

}

