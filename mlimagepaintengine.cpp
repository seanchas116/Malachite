#include <QtGui>
#include "agg_curves.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_scanline_p.h"

#include "mlimagepaintengine.h"

template <class BaseRenderer>
class MLRenderer
{
public:
	MLRenderer(BaseRenderer *ren) :
		_ren(ren)
	{}
	
	// from AGG code
    template<class Scanline> 
    void render(const Scanline& sl)
    {
        int y = sl.y();
        unsigned num_spans = sl.num_spans();
        typename Scanline::const_iterator span = sl.begin();

        for(;;)
        {
            int x = span->x;
            if(span->len > 0)
            {
                _ren->blendRasterizerSpan(x, y, (unsigned)span->len, 
                                      color, 
                                      span->covers);
            }
            else
            {
                _ren->blendRasterizerLine(x, y, (unsigned)(x - span->len - 1), 
                                color, 
                                *(span->covers));
            }
            if(--num_spans == 0) break;
            ++span;
        }
    }
	
private:
	BaseRenderer *_ren;
};

// from AGG code
template<class Rasterizer, class Scanline, class Renderer>
void mlRenderScanline(Rasterizer& ras, Scanline& sl, Renderer& ren)
{
    if(ras.rewind_scanlines())
    {
        sl.reset(ras.min_x(), ras.max_x());
        ren.prepare();
        while(ras.sweep_scanline(sl))
        {
            ren.render(sl);
        }
    }
}

template <class Filler>
class MLBaseRenderer
{
public:
	MLBaseRenderer(Filler *filler, const QRect &rect) :
		_filler(filler),
		_rect(rect)
	{}
	
	void blendRasterizerSpan(int x, int y, unsigned count, const uint8_t *covers)
	{
		if (y < _rect.top() || _rect.bottom() < y)
			return;
		
		int start = qMax(x, _rect.left());
		int end = qMin(x + count, _rect.left() + _rect.width());
		unsigned newCount = end - start;
		
		float *newCovers = new float[newCount];
		
		for (int i = 0; i < newCount; ++i)
		{
			newCovers[i] = (float)covers[i + start - x] / 255.0f;
		}
		
		_filler->blend(start, y, newCount, newCovers);
		
		delete[] newCovers;
	}
	
	void blendRasterizerLine(int x, int y, unsigned count, uint8_t cover)
	{
		if (y < _rect.top() || _rect.bottom() < y)
			return;
		
		int start = qMax(x, _rect.left());
		int end = qMin(x + count, _rect.left() + _rect.width());
		unsigned newCount = end - start;
		
		_filler->blend(start, y, newCount, (float)cover / 255.0f);
	}
	
private:
	Filler *_filler;
	QRect _rect;
};

class MLImageColorFiller
{
public:
	MLImageColorFiller(MLBitmap<MLFastArgbF> *bitmap, const MLFastArgbF &argb, double opacity) :
		_bitmap(bitmap),
		_argb(argb)
	{
		_argb.v *= mlFloatToVector(opacity);
	}
	
	void blend(int x, int y, unsigned count, float *covers);
	void blend(int x, int y, unsigned count, float cover);
	
private:
	MLBitmap<MLFastArgbF> _bitmap;
	MLFastArgbF _argb;
};

MLImagePaintEngine::MLImagePaintEngine() :
	MLPaintEngine(),
	_rasterizer(this)
{
}

bool MLImagePaintEngine::begin(MLPaintable *paintable)
{
	_image = dynamic_cast<MLImage *>(paintable);
	Q_ASSERT(_image);
	
	_bitmap = _image->bitmap();
	
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
	agg::scanline_p8 sl;
	agg::rasterizer_scanline_aa ras;
	
	
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

void MLImagePaintEngine::fillScanline(int x, int y, int count)
{
	if (_opacity == 1.0f)
		_blendOp->blend(count, pixelPointer(x, y), _state.argb);
	else
		_blendOp->blend(count, pixelPointer(x, y), _state.argb, _opacity);
}

