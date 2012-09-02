#include <QtGui>
#include "mlpolygon.h"
#include "mlpainter.h"

MLPainter::MLPainter(MLPaintable *paintable) :
	_paintable(0)
{
	bool ok = begin(paintable);
	if (!ok)
		qWarning() << "MLPainter::MLPainter: could not begin painting";
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

