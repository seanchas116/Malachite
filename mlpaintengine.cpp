
#include "mlpaintengine.h"

MLPaintEngine::MLPaintEngine()
{
	_state.blendMode = ML::BlendModeNormal;
	_state.brush = MLBrush(MLColor::fromRgbValue(0, 0, 0));
	_state.opacity = 1.0;
	_state.imageTransformType = ML::ImageTransformTypeBicubic;
}

void MLPaintEngine::drawTransformedPolygons(const MLMultiPolygon &polygons)
{
	drawTransformedPolygons(MLFixedMultiPolygon::fromMLPolygons(polygons));
}

void MLPaintEngine::drawPolygons(const MLMultiPolygon &polygons)
{
	drawTransformedPolygons(polygons * state()->shapeTransform);
}

void MLPaintEngine::drawPath(const QPainterPath &path)
{
	drawTransformedPolygons(MLFixedMultiPolygon::fromQPainterPath(path * state()->shapeTransform));
}

void MLPaintEngine::drawEllipse(double x, double y, double rx, double ry)
{
	QPainterPath path;
	path.addEllipse(QPointF(x, y), rx, ry);
	drawPath(path);
}

void MLPaintEngine::drawRect(double x, double y, double width, double height)
{
	MLPolygon polygon(4);
	
	polygon[0] = MLVec2D(x, y);
	polygon[1] = MLVec2D(x + width, y);
	polygon[2] = MLVec2D(x + width, y + height);
	polygon[3] = MLVec2D(x, y + height);
	
	drawPolygons(polygon);
}

void MLPaintEngine::drawTransformedSurface(const QPoint &point, const MLSurface &surface)
{
	foreach (const QPoint &key, surface.keys())
		drawTransformedImage(point + key * MLSurface::TileSize, surface.tileForKey(key));
}

void MLPaintEngine::drawSurface(const MLVec2D &point, const MLSurface &surface)
{
	QTransform transform = _state.shapeTransform;
	if (transform.type() <= QTransform::TxTranslate)
	{
		MLVec2D offset = point + MLVec2D(transform.dx(), transform.dy());
		QPoint rounded(round(offset.x), round(offset.y));
		
		if (offset.x == rounded.x() && offset.y == rounded.y())
		{
			drawTransformedSurface(rounded, surface);
			return;
		}
	}
	
	pushState();
	
	_state.shapeTransform = QTransform::fromTranslate(point.x, point.y) * _state.shapeTransform;
	_state.brush = MLBrush(surface);
	
	qDebug() << surface.keys();
	
	foreach (const QPoint &key, surface.keys())
	{
		MLVec2D relativePos = key * MLSurface::TileSize;
		drawRect(relativePos.x, relativePos.y, MLSurface::TileSize, MLSurface::TileSize);
	}
	
	popState();
}

void MLPaintEngine::drawImage(const MLVec2D &point, const MLImage &image)
{
	QTransform transform = _state.shapeTransform;
	if (transform.type() <= QTransform::TxTranslate)
	{
		MLVec2D offset = point + MLVec2D(transform.dx(), transform.dy());
		QPoint rounded(round(offset.x), round(offset.y));
		
		if (offset.x == rounded.x() && offset.y == rounded.y())
		{
			drawTransformedImage(rounded, image);
			return;
		}
	}
	
	pushState();
	
	_state.brush = MLBrush(image);
	_state.brush.setSpreadType(ML::SpreadTypeReflective);
	
	_state.shapeTransform = QTransform::fromTranslate(point.x, point.y) * _state.shapeTransform;
	
	drawRect(0, 0, image.width(), image.height());
	
	popState();
}

void MLPaintEngine::pushState()
{
	_stateStack.push(_state);
}

void MLPaintEngine::popState()
{
	_state = _stateStack.pop();
}
