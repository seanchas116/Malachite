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

void MLSurfacePaintEngine::drawPath(const QPainterPath &path)
{
	QRect boundingRect = _state.shapeTransform.mapRect(path.boundingRect()).toAlignedRect();
	
	foreach (const QPoint &key, MLSurface::keysForRect(boundingRect))
	{
		if (_keyClip.isEmpty() || _keyClip.contains(key))
		{
			MLPainter painter(_editor->tileRefForKey(key));
			painter.setState(_state);
			
			QPoint delta = -key * MLSurface::TileSize;
			painter.setShapeTransform(_state.shapeTransform * QTransform::fromTranslate(delta.x(), delta.y()));
			
			//painter.translateShape(-key * MLSurface::TileSize);
			
			painter.drawPath(path);
		}
	}
}

void MLSurfacePaintEngine::drawImage(const QPoint &point, const MLImage &image)
{
	QTransform transform = _state.shapeTransform;
	
	if (mlTransformIsSimilar(transform))
	{
		QPoint offset = QPoint(transform.dx(), transform.dy()) + point;
		
		foreach (const QPoint &key, MLSurface::keysForRect(QRect(point, image.size())))
		{
			if (_keyClip.isEmpty() || _keyClip.contains(key))
			{
				MLPainter painter(_editor->tileRefForKey(key));
				painter.setState(_state);
				
				painter.drawImage(offset - key * MLSurface::TileSize, image);
			}
		}
	}
	else
	{
		MLBrush brush = _state.brush;
		MLBrush imageBrush(image);
		imageBrush.setSpreadType(ML::SpreadTypeReflective);
		_state.brush = imageBrush;
		
		drawRect(point.x(), point.y(), image.width(), image.height());
		
		_state.brush = brush;
	}
}

void MLSurfacePaintEngine::updateState(const MLPaintEngineState &state)
{
	_state = state;
}


