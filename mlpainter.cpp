#include "mlpainter.h"

MLPainter::MLPainter(MLPaintable *paintable) :
	_paintable(0),
	_paintEngine(0)
{
	_state.blendMode = MLGlobal::BlendModeNormal;
	_state.brush = MLBrush(MLColor::fromRgb(0, 0, 0));
	_state.opacity = 1.0;
	bool ok = begin(paintable);
	if (!ok)
		qDebug() << "MLPainter::MLPainter: cannot begin painting";
}

MLPainter::~MLPainter()
{
	end();
}

bool MLPainter::begin(MLPaintable *paintable)
{
	if (_paintable)
		return false;
	
	_paintable = paintable;
	_paintEngine = _paintable->createPaintEngine();
	
	bool ok = _paintEngine->begin(_paintable);
	
	if (!ok) {
		_paintEngine = 0;
		return false;
	}
	
	_paintEngine->updateState(_state);
	return true;
}

void MLPainter::flush()
{
	if (!_paintEngine)
		return;
	_paintEngine->flush();
}

void MLPainter::end()
{
	flush();
	delete _paintEngine;
	_paintEngine = 0;
}

void MLPainter::drawPath(const QPainterPath &path)
{
	if (!_paintEngine)
		return;
	_paintEngine->drawPath(path);
}

void MLPainter::drawEllipse(const QRectF &rect)
{
	if (!_paintEngine)
		return;
	_paintEngine->drawEllipse(rect.x() + rect.width() / 2.0, rect.y() + rect.height() / 2.0, rect.width() / 2.0, rect.height() / 2.0);
}

void MLPainter::drawEllipse(const QPointF &center, double rx, double ry)
{
	if (!_paintEngine)
		return;
	_paintEngine->drawEllipse(center.x(), center.y(), rx, ry);
}

void MLPainter::drawImage(const QPoint &point, const MLImage &image)
{
	if (!_paintEngine)
		return;
	_paintEngine->drawImage(point, image);
}

void MLPainter::drawSurface(const QPoint &point, const MLSurface &surface)
{
	if (!_paintEngine)
		return;
	_paintEngine->drawSurface(point, surface);
}

