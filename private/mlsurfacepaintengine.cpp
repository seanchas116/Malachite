#include <QtGui>
#include <float.h>

#include "mlmisc.h"
#include "mlpainter.h"
#include "mlsurfacepainter.h"
#include "mlsurfacepaintengine.h"

MLSurfacePaintEngine::MLSurfacePaintEngine() :
	MLPaintEngine(),
	_editor(0)
{
}

MLSurfacePaintEngine::~MLSurfacePaintEngine()
{
	delete _editor;
}

bool MLSurfacePaintEngine::begin(MLPaintable *paintable)
{
	MLSurface *surface = dynamic_cast<MLSurface *>(paintable);
	if (!surface)
		return false;
	
	_editor = new MLSurfaceEditor(surface);
	return true;
}

bool MLSurfacePaintEngine::flush()
{
	return true;
}

void MLSurfacePaintEngine::drawTransformedPolygons(const MLFixedMultiPolygon &polygons)
{
	QRect boundingRect = polygons.boundingRect().toAlignedRect();
	
	QPointSet keys = MLSurface::keysForRect(boundingRect);
	if (!_keyClip.isEmpty())
		keys &= _keyClip;
	
	foreach (const QPoint &key, keys)
	{
		MLFixedMultiPolygon rectShape = MLFixedPolygon::fromRect(MLSurface::keyToRect(key));
		MLFixedMultiPolygon clippedShape = rectShape & polygons;
		
		QPoint delta = -key * MLSurface::TileSize;
		
		clippedShape.translate(delta);
		
		MLPainter painter(_editor->tileRefForKey(key));
		*painter.state() = *state();
		painter.setShapeTransform(state()->shapeTransform * QTransform::fromTranslate(delta.x(), delta.y()));
		
		painter.drawTransformedPolygons(clippedShape);
	}
}

void MLSurfacePaintEngine::drawTransformedImage(const QPoint &point, const MLImage &image)
{
	QPointSet keys = MLSurface::keysForRect(QRect(point, image.size()));
	if (!_keyClip.isEmpty())
		keys &= _keyClip;
	
	foreach (const QPoint &key, keys)
	{
		MLPainter painter(_editor->tileRefForKey(key));
		*painter.state() = *state();
		painter.drawTransformedImage(point - key * MLSurface::TileSize, image);
	}
}

void MLSurfacePaintEngine::drawTransformedSurface(const QPoint &point, const MLSurface &surface)
{
	if (point == QPoint())
	{
		QPointSet keys = surface.keys() & _editor->surface()->keys();
		
		if (!_keyClip.isEmpty())
			keys &= _keyClip;
		
		foreach (const QPoint &key, keys)
		{
			MLBlendOp::TileCombination combination = MLBlendOp::NoTile;
			
			if (_editor->surface()->contains(key))
				combination |= MLBlendOp::TileDestination;
			if (surface.contains(key))
				combination |= MLBlendOp::TileSource;
			
			switch (state()->blendMode.op()->tileRequirement(combination))
			{
				case MLBlendOp::TileSource:
					*_editor->tileRefForKey(key) = surface.tileForKey(key) * state()->opacity;
					break;
					
				case MLBlendOp::NoTile:
					_editor->deleteTile(key);
					break;
					
				default:
				case MLBlendOp::TileDestination:
					break;
					
				case MLBlendOp::TileBoth:
					
					MLPainter painter(_editor->tileRefForKey(key));
					painter.setBlendMode(state()->blendMode);
					painter.setOpacity(state()->opacity);
					painter.drawTransformedImage(QPoint(), surface.tileForKey(key));
					break;
			}
		}
	}
	else
	{
		foreach (const QPoint &key, surface.keys())
			drawTransformedImage(point + key * MLSurface::TileSize, surface.tileForKey(key));
	}
}

