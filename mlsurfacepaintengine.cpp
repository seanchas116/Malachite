#include <QtGui>

#include "mlmisc.h"
#include "mlpainter.h"
#include "mlsurfacepaintengine.h"

MLSurfacePaintEngine::MLSurfacePaintEngine() :
	MLPaintEngine()
{
}

MLSurfacePaintEngine::~MLSurfacePaintEngine()
{
	squeeze();
}

bool MLSurfacePaintEngine::begin(MLPaintable *paintable)
{
	_surface = dynamic_cast<MLSurface *>(paintable);
	if (!_surface)
		return false;
	
	_surface->setupData();
	return true;
}

bool MLSurfacePaintEngine::flush()
{
	return true;
}

void MLSurfacePaintEngine::drawEllipse(double x, double y, double rx, double ry)
{
	QRectF rect(x - rx, y - ry, 2 * rx, 2 * ry);
	QPointSet keys = MLSurface::keysForRect(rect.toAlignedRect());
	
	foreach (const QPoint &key, keys) {
		QPointF realCenter = QPointF(x, y) - key * MLSurface::TileSize;
		MLPainter painter(tileRefForKey(key));
		painter.setState(_state);
		painter.drawEllipse(realCenter, rx, ry);
	}
}

void MLSurfacePaintEngine::drawPath(const QPainterPath &path)
{
	foreach (const QPoint &key, MLSurface::keysForRect(path.boundingRect().toAlignedRect())) {
		if (_keyClip.isEmpty() || _keyClip.contains(key)) {
			MLPainter painter(tileRefForKey(key));
			painter.setState(_state);
			painter.drawPath(path.translated(-key * MLSurface::TileSize));
		}
	}
}

void MLSurfacePaintEngine::drawImage(const QPoint &point, const MLImage &image)
{
	foreach (const QPoint &key, MLSurface::keysForRect(QRect(point, image.size()))) {
		
		if (_keyClip.isEmpty() || _keyClip.contains(key)) {
			MLPainter painter(tileRefForKey(key));
			painter.setState(_state);
			painter.drawImage(point - key * MLSurface::TileSize, image);
		}
	}
}

void MLSurfacePaintEngine::deleteTile(const QPoint &key)
{
	delete takeTile(key);
}

void MLSurfacePaintEngine::clear()
{
	_editedKeys += _surface->keys();
	qDeleteAll(_surface->d->tileHash);
	_surface->d->tileHash.clear();
}

MLImage *MLSurfacePaintEngine::takeTile(const QPoint &key)
{
	if (!_surface->d->tileHash.contains(key))
		return 0;
	_editedKeys << key;
	return _surface->d->tileHash.take(key);
}

MLImage *MLSurfacePaintEngine::replaceTile(const QPoint &key, MLImage *image)
{
	_editedKeys << key;
	MLImage *tile = takeTile(key);
	_surface->d->tileHash.insert(key, image);
	return tile;
}

void MLSurfacePaintEngine::replace(const MLSurface &surface, const QPointSet &keys)
{
	foreach (const QPoint &key, keys) {
		MLImage *oldTile = replaceTile(key, new MLImage(surface.tileForKey(key)));
		if (oldTile)
			delete oldTile;
	}
}

void MLSurfacePaintEngine::updateState(const MLPaintEngineState &state)
{
	_state = state;
}

// Returns tile item with "key"
// If the tile hash no tile with "key", inserts a new tile and returns it
MLImage *MLSurfacePaintEngine::tileRefForKey(const QPoint &key)
{
	MLImage *tile;
	
	if (!_surface->d->tileHash.contains(key)) {
		tile = new MLImage(MLSurface::DefaultTile);
		_surface->d->tileHash.insert(key, tile);
	} else {
		tile = _surface->d->tileHash[key];
	}
	_editedKeys << key;
	return tile;
}

const MLImage *MLSurfacePaintEngine::constTileRefForKey(const QPoint &key) const
{
	if (_surface->d->tileHash.contains(key))
		return _surface->d->tileHash[key];
	else
		return &MLSurface::DefaultTile;
}

// Deletes blank tiles
void MLSurfacePaintEngine::squeeze()
{
	QPointList deleteList;
	
	foreach (const QPoint &key, _editedKeys) {
		if (_surface->d->tileHash.contains(key)) {
			if (imageIsBlank(*(_surface->d->tileHash.value(key))))
				deleteList << key;
		}
	}
	
	foreach (const QPoint &key, deleteList) {
		delete _surface->d->tileHash.take(key);
	}
}

// Checks whether the image is blank (i.e., alpha value of any pixel on the image is 0)
bool MLSurfacePaintEngine::imageIsBlank(const MLImage &image)
{
	for (int y = 0; y < image.height(); ++y) {
		const MLArgb *p = image.constScanline(y);
		for (int x = 0; x < image.width(); ++x) {
			if (p->a())
				return false;
			p++;
		}
	}
	return true;
}


