#include <QtGui>

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
	
	foreach (const QPoint &key, MLSurface::keysForRect(boundingRect))
	{
		if (_keyClip.isEmpty() || _keyClip.contains(key))
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
}

void MLSurfacePaintEngine::drawTransformedImage(const QPoint &point, const MLImage &image)
{
	foreach (const QPoint &key, MLSurface::keysForRect(QRect(point, image.size())))
	{
		if (_keyClip.isEmpty() || _keyClip.contains(key))
		{
			MLPainter painter(_editor->tileRefForKey(key));
			*painter.state() = *state();
			painter.drawTransformedImage(point - key * MLSurface::TileSize, image);
		}
	}
}

