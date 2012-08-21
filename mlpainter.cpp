#include <QtGui>
#include "mlpolygon.h"
#include "mlpainter.h"

MLPainter::MLPainter(MLPaintable *paintable) :
	_paintable(0)
{
	_state.blendMode = ML::BlendModeNormal;
	_state.brush = MLBrush(MLColor::fromRgbValue(0, 0, 0));
	_state.opacity = 1.0;
	_state.imageTransformType = ML::ImageTransformTypeBicubic;
	bool ok = begin(paintable);
	if (!ok)
		qDebug() << "MLPainter::MLPainter: could not begin painting";
}

MLPainter::~MLPainter()
{
	end();
}

bool MLPainter::begin(MLPaintable *paintable)
{
	if (_paintable) return false;
	
	_paintable = paintable;
	_paintEngine.reset(_paintable->createPaintEngine());
	
	bool ok = _paintEngine->begin(_paintable);
	
	if (!ok) {
		_paintEngine.reset();
		return false;
	}
	
	_paintEngine->updateState(_state);
	return true;
}

void MLPainter::flush()
{
	if (!_paintEngine) return;
	_paintEngine->flush();
}

void MLPainter::end()
{
	flush();
	_paintEngine.reset();
}

void MLPainter::drawPath(const QPainterPath &path)
{
	if (!_paintEngine) return;
	_paintEngine->drawPath(path);
}

void MLPainter::drawPolygon(const MLPolygon &polygon)
{
	QPainterPath path;
	path.addPolygon(polygon.toQPolygonF());
	drawPath(path);
}

void MLPainter::drawEllipse(const QRectF &rect)
{
	if (!_paintEngine) return;
	_paintEngine->drawEllipse(rect.x() + rect.width() / 2.0, rect.y() + rect.height() / 2.0, rect.width() / 2.0, rect.height() / 2.0);
}

void MLPainter::drawEllipse(double x, double y, double rx, double ry)
{
	if (!_paintEngine) return;
	_paintEngine->drawEllipse(x, y, rx, ry);
}

void MLPainter::drawRect(const QRectF &rect)
{
	if (!_paintEngine) return;
	_paintEngine->drawRect(rect.x(), rect.y(), rect.width(), rect.height());
}

void MLPainter::drawImage(const QPoint &point, const MLImage &image)
{
	if (!_paintEngine) return;
	_paintEngine->drawImage(point, image);
}

void MLPainter::drawSurface(const QPoint &point, const MLSurface &surface)
{
	if (!_paintEngine) return;
	_paintEngine->drawSurface(point, surface);
}

