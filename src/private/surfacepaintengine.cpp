#include <QtGui>
#include <float.h>

#include "./misc.h"
#include "./painter.h"
#include "./surfacepainter.h"
#include "surfacepaintengine.h"

namespace Malachite
{

SurfacePaintEngine::SurfacePaintEngine() :
	PaintEngine(),
	_editor(0)
{
}

SurfacePaintEngine::~SurfacePaintEngine()
{
	delete _editor;
}

bool SurfacePaintEngine::begin(Paintable *paintable)
{
	Surface *surface = dynamic_cast<Surface *>(paintable);
	if (!surface)
		return false;
	
	_editor = new SurfaceEditor(surface);
	return true;
}

bool SurfacePaintEngine::flush()
{
	return true;
}

void SurfacePaintEngine::drawTransformedPolygons(const FixedMultiPolygon &polygons)
{
	QRect boundingRect = polygons.boundingRect().toAlignedRect();
	
	QPointSet keys = Surface::keysForRect(boundingRect);
	if (!_keyClip.isEmpty())
		keys &= _keyClip;
	
	foreach (const QPoint &key, keys)
	{
		FixedMultiPolygon rectShape = FixedPolygon::fromRect(Surface::keyToRect(key));
		FixedMultiPolygon clippedShape = rectShape & polygons;
		
		QPoint delta = -key * Surface::TileSize;
		
		clippedShape.translate(delta);
		
		Painter painter(_editor->tileRefForKey(key));
		*painter.state() = *state();
		painter.setShapeTransform(state()->shapeTransform * QTransform::fromTranslate(delta.x(), delta.y()));
		
		painter.drawTransformedPolygons(clippedShape);
	}
}

void SurfacePaintEngine::drawTransformedImage(const QPoint &point, const Image &image)
{
	QPointSet keys = Surface::keysForRect(QRect(point, image.size()));
	if (!_keyClip.isEmpty())
		keys &= _keyClip;
	
	foreach (const QPoint &key, keys)
	{
		Painter painter(_editor->tileRefForKey(key));
		*painter.state() = *state();
		painter.drawTransformedImage(point - key * Surface::TileSize, image);
	}
}

void SurfacePaintEngine::drawTransformedSurface(const QPoint &point, const Surface &surface)
{
	if (point == QPoint())
	{
		QPointSet keys = surface.keys() & _editor->surface()->keys();
		
		if (!_keyClip.isEmpty())
			keys &= _keyClip;
		
		foreach (const QPoint &key, keys)
		{
			BlendOp::TileCombination combination = BlendOp::NoTile;
			
			if (_editor->surface()->contains(key))
				combination |= BlendOp::TileDestination;
			if (surface.contains(key))
				combination |= BlendOp::TileSource;
			
			switch (BlendModeUtil(state()->blendMode).op()->tileRequirement(combination))
			{
				case BlendOp::TileSource:
					*_editor->tileRefForKey(key) = surface.tileForKey(key) * state()->opacity;
					break;
					
				case BlendOp::NoTile:
					_editor->deleteTile(key);
					break;
					
				default:
				case BlendOp::TileDestination:
					break;
					
				case BlendOp::TileBoth:
					
					Painter painter(_editor->tileRefForKey(key));
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
			drawTransformedImage(point + key * Surface::TileSize, surface.tileForKey(key));
	}
}

}

