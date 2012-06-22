#include <QtGui>

#include "mlmisc.h"
#include "mlpainter.h"
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

void MLSurfacePaintEngine::drawPath(const QPainterPath &path)
{
	foreach (const QPoint &key, MLSurface::keysForRect(path.boundingRect().toAlignedRect()))
	{
		if (_keyClip.isEmpty() || _keyClip.contains(key))
		{
			MLPainter painter(_editor->tileRefForKey(key));
			painter.setState(_state);
			
			MLBrush brush = _state.brush;
			
			QPoint offset = -key * MLSurface::TileSize;
			brush.setTransform(brush.transform() * QTransform::fromTranslate(offset.x(), offset.y()));
			
			painter.setBrush(brush);
			
			painter.drawPath(path.translated(-key * MLSurface::TileSize));
		}
	}
}

void MLSurfacePaintEngine::drawImage(const QPoint &point, const MLImage &image)
{
	foreach (const QPoint &key, MLSurface::keysForRect(QRect(point, image.size())))
	{
		if (_keyClip.isEmpty() || _keyClip.contains(key))
		{
			MLPainter painter(_editor->tileRefForKey(key));
			painter.setState(_state);
			
			painter.drawImage(point - key * MLSurface::TileSize, image);
		}
	}
}

void MLSurfacePaintEngine::updateState(const MLPaintEngineState &state)
{
	_state = state;
}


