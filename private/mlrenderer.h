#ifndef MLRENDERER_H
#define MLRENDERER_H

#include <QPainterPath>
#include "agg_rasterizer_scanline_aa.h"
#include "agg_scanline_p.h"
#include "mlcurvesubdivision.h"
#include "mlbitmap.h"
#include "mlblendop.h"

typedef MLBitmap<MLVec4F> MLArgbBitmap;

class QPainterPath_vs
{
public:
	QPainterPath_vs(const QPainterPath &path) :
		_path(path),
		_index(0),
		_subdIndex(0)
	{}
	
	void rewind(unsigned index) { Q_UNUSED(index); _index = 0; }
	unsigned vertex(double *x, double *y);
	
private:
	const QPainterPath _path;
	int _index;
	MLPolygon _subdPolygon;
	int _subdIndex;
};

template <class BaseRenderer>
class MLRenderer
{
public:
	MLRenderer(BaseRenderer &ren) :
		_ren(&ren)
	{}
	
	// from agg code
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
                _ren->blendRasterizerSpan(x, y,span->len, span->covers);
            }
            else
            {
                _ren->blendRasterizerLine(x, y, -span->len, *(span->covers));
            }
            if(--num_spans == 0) break;
            ++span;
        }
    }
	
private:
	BaseRenderer *_ren;
};

template<class Rasterizer, class Scanline, class Renderer>
void mlRenderScanlines(Rasterizer& ras, Scanline& sl, Renderer& ren)
{
    if(ras.rewind_scanlines())
    {
        sl.reset(ras.min_x(), ras.max_x());
        while(ras.sweep_scanline(sl))
        {
            ren.render(sl);
        }
    }
}

template <class Filler>
class MLImageBaseRenderer
{
public:
	MLImageBaseRenderer(const MLArgbBitmap &bitmap, MLBlendOp *blendOp, Filler *filler) :
		_bitmap(bitmap),
		_blendOp(blendOp),
		_filler(filler)
	{}
	
	void blendRasterizerSpan(int x, int y, int count, const uint8_t *covers)
	{
		if (y < _bitmap.rect().top() || _bitmap.rect().bottom() < y)
			return;
		
		int start = qMax(x, _bitmap.rect().left());
		int end = qMin(x + count, _bitmap.rect().left() + _bitmap.rect().width());
		int newCount = end - start;
		
		if (newCount <= 0)
			return;
		
		MLArray<float> newCovers(newCount);
		
		for (int i = 0; i < newCount; ++i)
		{
			newCovers[i] = (float)covers[i + start - x] * (1.f / 255.f);
		}
		
		QPoint pos(start, y);
		_filler->fill(pos, newCount, _bitmap.pixelPointer(pos), newCovers.data(), _blendOp);
	}
	
	void blendRasterizerLine(int x, int y, int count, uint8_t cover)
	{
		if (y < _bitmap.rect().top() || _bitmap.rect().bottom() < y)
			return;
		
		int start = qMax(x, _bitmap.rect().left());
		int end = qMin(x + count, _bitmap.rect().left() + _bitmap.rect().width());
		int newCount = end - start;
		
		if (newCount <= 0)
			return;
		
		QPoint pos(start, y);
		_filler->fill(pos, newCount, _bitmap.pixelPointer(pos), float(cover) * (1.f / 255.f), _blendOp);
	}
	
private:
	MLArgbBitmap _bitmap;
	MLBlendOp *_blendOp;
	Filler *_filler;
};


#endif // MLRENDERER_H
