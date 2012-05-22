#include <QtGui>
#include "agg_curves.h"

#include "mlimagepaintengine.h"

MLImagePaintEngine::MLImagePaintEngine() :
	MLPaintEngine(),
	_rasterizer(this)
{
}

bool MLImagePaintEngine::begin(MLPaintable *paintable)
{
	_image = dynamic_cast<MLImage *>(paintable);
	Q_ASSERT(_image);
	_bits = _image->bits();
	_size = _image->size();
	_bytesPerLine = _image->bytesPerLine();
	return true;
}

bool MLImagePaintEngine::flush()
{
	return true;
}

void MLImagePaintEngine::updateState(const MLPaintEngineState &state)
{
	_state = state;
	_blendOp = state.blendMode.op();
	_opacity = state.opacity;
}

void MLImagePaintEngine::drawPath(const QPainterPath &path)
{
	_rasterizer.rasterize(path);
}

void MLImagePaintEngine::drawImage(const QPoint &point, const MLImage &image)
{
	QRect dstRect = QRect(QPoint(), _image->size());
	QRect srcRect = QRect(point, image.size());
	
	QRect targetRect = dstRect & srcRect;
	
	if (targetRect.isEmpty())
		return;
	
	for (int y = targetRect.top(); y <= targetRect.bottom(); ++y)
	{
		QPoint p(targetRect.left(), y);
		
		if (_opacity == 1.0f)
			_blendOp->blend(targetRect.width(), pixelPointer(p), image.constPixelPointer(p - point));
		else
			_blendOp->blend(targetRect.width(), pixelPointer(p), image.constPixelPointer(p - point), _opacity);
	}
}

void MLImagePaintEngine::fillScanline(int x, int y, int count, float *covers)
{
	if (_opacity != 1.0f)
	{
		for (int i = 0; i < count; ++i)
		{
			covers[i] *= _opacity;
		}
	}
	
	_blendOp->blend(count, pixelPointer(x, y), _state.argb, covers);
}

