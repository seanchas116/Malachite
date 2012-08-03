#include "mlpaintengine.h"

void MLPaintEngine::drawPath(const QPainterPath &path)
{
	Q_UNUSED(path);
}

void MLPaintEngine::drawEllipse(double x, double y, double rx, double ry)
{
	QPainterPath path;
	path.addEllipse(QPointF(x, y), rx, ry);
	drawPath(path);
}

void MLPaintEngine::drawRect(double x, double y, double width, double height)
{
	QPainterPath path;
	path.addRect(x, y, width, height);
	drawPath(path);
}

void MLPaintEngine::drawImage(const QPoint &point, const MLImage &image)
{
	Q_UNUSED(point);
	Q_UNUSED(image);
}

void MLPaintEngine::drawSurface(const QPoint &point, const MLSurface &surface)
{
	foreach (const QPoint &key, surface.keys()) {
		drawImage(point + key * MLSurface::TileSize, surface.tileForKey(key));
	}
}
