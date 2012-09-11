#include <QtGui>
#include "mlpolygon.h"
#include "mlpainter.h"

namespace Malachite
{

Painter::Painter(Paintable *paintable) :
	_paintable(0)
{
	bool ok = begin(paintable);
	if (!ok)
		qWarning() << "MLPainter::MLPainter: could not begin painting";
}

Painter::~Painter()
{
	end();
}

bool Painter::begin(Paintable *paintable)
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

void Painter::flush()
{
	if (!_paintEngine) return;
	_paintEngine->flush();
}

void Painter::end()
{
	flush();
	_paintEngine.reset();
}

}
