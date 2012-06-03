//----------------------------------------------------------------------------
// Anti-Grain Geometry - Version 2.4
// Copyright (C) 2002-2005 Maxim Shemanarev (http://www.antigrain.com)
//
// Permission to copy, use, modify, sell and distribute this software 
// is granted provided this copyright notice appears in all copies. 
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.
//
//----------------------------------------------------------------------------
// Contact: mcseem@antigrain.com
//          mcseemagg@yahoo.com
//          http://www.antigrain.com
//----------------------------------------------------------------------------

#ifndef MLIMAGERENDERER_H
#define MLIMAGERENDERER_H

#include <QPainterPath>
#include "mlimage.h"
#include "mlpainter.h"
#include "agg_curves.h"

class QPainterPath_vs
{
public:
	QPainterPath_vs(const QPainterPath &path) :
		_path(path),
		_index(0),
		_curve4(0)
	{}
	
	void rewind(unsigned index) { Q_UNUSED(index); _index = 0; }
	unsigned vertex(double *x, double *y);
	
private:
	const QPainterPath _path;
	int _index;
	agg::curve4_div *_curve4;
};

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

// from AGG code
template<class Rasterizer, class Scanline, class Renderer>
void render_scanlines(Rasterizer& ras, Scanline& sl, Renderer& ren)
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
	MLImageBaseRenderer(const MLBitmap<MLArgb> &bitmap, MLBlendOp *blendOp, Filler *filler) :
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
		
		float *newCovers = new float[newCount];
		
		for (int i = 0; i < newCount; ++i)
		{
			newCovers[i] = (float)covers[i + start - x] / 255.0f;
		}
		
		_filler->blend(_bitmap, _blendOp, start, y, newCount, newCovers);
		
		delete[] newCovers;
	}
	
	void blendRasterizerLine(int x, int y, int count, uint8_t cover)
	{
		if (y < _bitmap.rect().top() || _bitmap.rect().bottom() < y)
			return;
		
		int start = qMax(x, _bitmap.rect().left());
		int end = qMin(x + count, _bitmap.rect().left() + _bitmap.rect().width());
		unsigned newCount = end - start;
		
		_filler->blend(_bitmap, _blendOp, start, y, newCount, (float)cover / 255.0f);
	}
	
private:
	MLBitmap<MLArgb> _bitmap;
	MLBlendOp *_blendOp;
	Filler *_filler;
};


class MLImageColorFiller
{
public:
	MLImageColorFiller(const MLArgb &argb, double opacity) :
		_argb(argb)
	{
		_argb.v *= opacity;
	}
	
	void blend(MLBitmap<MLArgb> &bitmap, MLBlendOp *blendOp, int x, int y, int count, float *covers)
	{
		blendOp->blend(count, bitmap.pixelPointer(x, y), _argb, covers);
	}
	
	void blend(MLBitmap<MLArgb> &bitmap, MLBlendOp *blendOp, int x, int y, int count, float cover)
	{
		blendOp->blend(count, bitmap.pixelPointer(x, y), _argb, cover);
	}
	
private:
	MLArgb _argb;
};

#endif // MLIMAGERENDERER_H
