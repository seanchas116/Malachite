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
#include "mlintdivision.h"
#include "mlimage.h"
#include "mlpainter.h"
#include "mlfillgenerator.h"
#include "mlcurvesubdivision.h"

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

namespace agg
{

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

}

class MLFiller
{
public:
	virtual void blend(MLBitmap<MLArgb> &bitmap, MLBlendOp *blendOp, const QPoint &point, int count, float *covers) = 0;
	virtual void blend(MLBitmap<MLArgb> &bitmap, MLBlendOp *blendOp, const QPoint &point, int count, float cover) = 0;
};

class MLImageBaseRenderer
{
public:
	MLImageBaseRenderer(const MLBitmap<MLArgb> &bitmap, MLBlendOp *blendOp, MLFiller &filler) :
		_bitmap(bitmap),
		_blendOp(blendOp),
		_filler(&filler)
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
		
		_filler->blend(_bitmap, _blendOp, QPoint(start, y), newCount, newCovers);
		
		delete[] newCovers;
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
		
		_filler->blend(_bitmap, _blendOp, QPoint(start, y), newCount, (float)cover / 255.0f);
	}
	
private:
	MLBitmap<MLArgb> _bitmap;
	MLBlendOp *_blendOp;
	MLFiller *_filler;
};

class MLColorFiller : public MLFiller
{
public:
	MLColorFiller(const MLArgb &argb, double opacity) :
		_argb(argb)
	{
		_argb.v *= opacity;
	}
	
	void blend(MLBitmap<MLArgb> &bitmap, MLBlendOp *blendOp, const QPoint &point, int count, float *covers)
	{
		blendOp->blend(count, bitmap.pixelPointer(point), _argb, covers);
	}
	
	void blend(MLBitmap<MLArgb> &bitmap, MLBlendOp *blendOp, const QPoint &point, int count, float cover)
	{
		MLArgb actualArgb = _argb;
		actualArgb.v *= cover;
		blendOp->blend(count, bitmap.pixelPointer(point), _argb);
	}
	
private:
	MLArgb _argb;
};


class MLGeneratedFiller : public MLFiller
{
public:
	
	MLGeneratedFiller(MLFillGenerator &generator, float opacity, const QTransform &transform = QTransform()) :
		_generator(&generator),
		_opacity(opacity),
		_transform(transform)
	{}
	
	void blend(MLBitmap<MLArgb> &bitmap, MLBlendOp *blendOp, const QPoint &point, int count, float *covers)
	{
		if (_opacity != 1.0)
		{
			for (int i = 0; i < count; ++i)
			{
				covers[i] *= _opacity;
			}
		}
		
		MLArgb *fill = new MLArgb[count];
		
		for (int i = 0; i < count; ++i)
		{
			QPointF p(point.x() + i + 0.5, point.y() + 0.5);
			fill[i] = _transform.isIdentity() ? _generator->at(p) : _generator->at(_transform * p);
		}
		
		blendOp->blend(count, bitmap.pixelPointer(point), fill, covers);
		
		delete fill;
	}
	
	void blend(MLBitmap<MLArgb> &bitmap, MLBlendOp *blendOp, const QPoint &point, int count, float cover)
	{
		cover *= _opacity;
		
		MLArgb *fill = new MLArgb[count];
		
		for (int i = 0; i < count; ++i)
		{
			QPointF p(point.x() + i + 0.5, point.y() + 0.5);
			fill[i] = _transform.isIdentity() ? _generator->at(p) : _generator->at(_transform * p);
			fill[i].v *= cover;
		}
		
		blendOp->blend(count, bitmap.pixelPointer(point), fill);
		
		delete fill;
	}
	
private:
	MLFillGenerator *_generator;
	float _opacity;
	QTransform _transform;
};


class MLPadImageFiller : public MLFiller
{
public:
	MLPadImageFiller(const MLBitmap<MLArgb> &bitmap, const QPoint &offset) :
		_srcBitmap(bitmap), _offset(offset) {}
	
	void blend(MLBitmap<MLArgb> &bitmap, MLBlendOp *blendOp, const QPoint &point, int count, float *covers)
	{
		QPoint srcPoint = point - _offset;
		int imageY = qBound(0, srcPoint.y(), _srcBitmap.height() - 1);
		
		int i = 0;
		
		int left = -srcPoint.x();
		
		if (left > 0)
		{
			blendOp->blend(left, bitmap.pixelPointer(point), _srcBitmap.pixel(0, imageY), covers);
			i += left;
		}
		
		int midStart = qMax(0, srcPoint.x());
		int midEnd = qMin(_srcBitmap.width(), srcPoint.x() + count);
		int mid = midEnd - midStart;
		
		if (mid > 0)
		{
			blendOp->blend(mid, bitmap.pixelPointer(point + QPoint(i, 0)), _srcBitmap.constPixelPointer(midStart, imageY), covers + i);
			i += mid;
		}
		
		int right = srcPoint.x() + count - _srcBitmap.width();
		
		if (right > 0)
		{
			blendOp->blend(mid, bitmap.pixelPointer(point + QPoint(i, 0)), _srcBitmap.pixel(_srcBitmap.width() - 1, imageY), covers + i);
		}
	}
	
	void blend(MLBitmap<MLArgb> &bitmap, MLBlendOp *blendOp, const QPoint &point, int count, float cover)
	{
		QPoint srcPoint = point - _offset;
		int imageY = qBound(0, srcPoint.y(), _srcBitmap.height() - 1);
		
		int i = 0;
		
		int left = srcPoint.x() < 0 ? -srcPoint.x() : 0;
		
		if (left > 0)
		{
			blendOp->blend(left, bitmap.pixelPointer(point), _srcBitmap.pixel(0, imageY), cover);
			i += left;
		}
		
		int midStart = qMax(0, srcPoint.x());
		int midEnd = qMin(_srcBitmap.width(), srcPoint.x() + count);
		int mid = midEnd - midStart;
		
		if (mid > 0)
		{
			blendOp->blend(mid, bitmap.pixelPointer(point + QPoint(i, 0)), _srcBitmap.constPixelPointer(midStart, imageY), cover);
			i += mid;
		}
		
		int right = srcPoint.x() + count - _srcBitmap.width();
		
		if (right > 0)
		{
			blendOp->blend(mid, bitmap.pixelPointer(point + QPoint(i, 0)), _srcBitmap.pixel(_srcBitmap.width() - 1, imageY), cover);
		}
	}
	
private:
	
	const MLBitmap<MLArgb> _srcBitmap;
	QPoint _offset;
};

class MLRepeatImageFiller : public MLFiller
{
public:
	MLRepeatImageFiller(const MLBitmap<MLArgb> &bitmap, const QPoint &offset) :
		_srcBitmap(bitmap), _offset(offset) {}
	
	void blend(MLBitmap<MLArgb> &bitmap, MLBlendOp *blendOp, const QPoint &point, int count, float *covers)
	{
		QPoint srcPoint = point - _offset;
		MLIntDivision divX(srcPoint.x(), _srcBitmap.width());
		MLIntDivision divY(srcPoint.y(), _srcBitmap.height());
		
		int imageY = divY.rem();
		
		int i = _srcBitmap.width() - divX.rem();
		
		if (i >= count)
		{
			blendOp->blend(count, bitmap.pixelPointer(point), _srcBitmap.constPixelPointer(divX.rem(), imageY), covers);
			return;
		}
		
		blendOp->blend(i, bitmap.pixelPointer(point), _srcBitmap.constPixelPointer(divX.rem(), imageY), covers);
		
		forever
		{
			if (count - i < _srcBitmap.width()) break;
			blendOp->blend(_srcBitmap.width(), bitmap.pixelPointer(point + QPoint(i, 0)), _srcBitmap.constPixelPointer(0, imageY), covers + i);
			i += _srcBitmap.width();
		}
		
		blendOp->blend(count - i, bitmap.pixelPointer(point + QPoint(i, 0)), _srcBitmap.constPixelPointer(0, imageY), covers + i);
	}
	
	void blend(MLBitmap<MLArgb> &bitmap, MLBlendOp *blendOp, const QPoint &point, int count, float cover)
	{
		QPoint srcPoint = point - _offset;
		MLIntDivision divX(srcPoint.x(), _srcBitmap.width());
		MLIntDivision divY(srcPoint.y(), _srcBitmap.height());
		
		int imageY = divY.rem();
		
		int i = _srcBitmap.width() - divX.rem();
		
		if (i >= count)
		{
			blendOp->blend(count, bitmap.pixelPointer(point), _srcBitmap.constPixelPointer(divX.rem(), imageY), cover);
			return;
		}
		
		blendOp->blend(i, bitmap.pixelPointer(point), _srcBitmap.constPixelPointer(divX.rem(), imageY), cover);
		
		forever
		{
			if (count - i < _srcBitmap.width()) break;
			blendOp->blend(_srcBitmap.width(), bitmap.pixelPointer(point + QPoint(i, 0)), _srcBitmap.constPixelPointer(0, imageY), cover);
			i += _srcBitmap.width();
		}
		
		blendOp->blend(count - i, bitmap.pixelPointer(point + QPoint(i, 0)),  _srcBitmap.constPixelPointer(0, imageY), cover);
	}
	
private:
	
	const MLBitmap<MLArgb> _srcBitmap;
	QPoint _offset;
};

class MLReflectiveImageFiller : public MLFiller
{
public:
	MLReflectiveImageFiller(const MLBitmap<MLArgb> &bitmap, const QPoint &offset) :
		_srcBitmap(bitmap), _offset(offset) {}
	
	void blend(MLBitmap<MLArgb> &bitmap, MLBlendOp *blendOp, const QPoint &point, int count, float *covers)
	{
		QPoint srcPoint = point - _offset;
		MLIntDivision divX(srcPoint.x(),  _srcBitmap.width());
		MLIntDivision divY(srcPoint.y(),  _srcBitmap.height());
		
		int imageY = divY.quot() % 2 ?  _srcBitmap.height() - divY.rem() - 1 : divY.rem();
		
		int i = _srcBitmap.width() - divX.rem();
		
		int q = divX.quot();
		
		if (i >= count)
		{
			if (q % 2)
				blendOp->blendReversed(count, bitmap.pixelPointer(point), _srcBitmap.constPixelPointer( _srcBitmap.width() - divX.rem() - count, imageY), covers);
			else
				blendOp->blend(count, bitmap.pixelPointer(point), _srcBitmap.constPixelPointer(divX.rem(), imageY), covers);
			return;
		}
		
		if (q % 2)
			blendOp->blendReversed(i, bitmap.pixelPointer(point), _srcBitmap.constPixelPointer(0, imageY), covers);
		else
			blendOp->blend(i, bitmap.pixelPointer(point), _srcBitmap.constPixelPointer(divX.rem(), imageY), covers);
		
		q++;
		
		forever
		{
			if (count - i < _srcBitmap.width()) break;
			
			if (q % 2)
				blendOp->blendReversed(_srcBitmap.width(), bitmap.pixelPointer(point + QPoint(i, 0)), _srcBitmap.constPixelPointer(0, imageY), covers + i);
			else
				blendOp->blend(_srcBitmap.width(), bitmap.pixelPointer(point + QPoint(i, 0)), _srcBitmap.constPixelPointer(0, imageY), covers + i);
			i += _srcBitmap.width();
			q++;
		}
		
		if (q % 2)
			blendOp->blendReversed(count - i, bitmap.pixelPointer(point + QPoint(i, 0)), _srcBitmap.constPixelPointer(_srcBitmap.width() - count + i, imageY), covers + i);
		else
			blendOp->blend(count - i, bitmap.pixelPointer(point + QPoint(i, 0)), _srcBitmap.constPixelPointer(0, imageY), covers + i);
	}
	
	void blend(MLBitmap<MLArgb> &bitmap, MLBlendOp *blendOp, const QPoint &point, int count, float cover)
	{
		QPoint srcPoint = point - _offset;
		MLIntDivision divX(srcPoint.x(), _srcBitmap.width());
		MLIntDivision divY(srcPoint.y(),  _srcBitmap.height());
		
		int imageY = divY.quot() % 2 ?  _srcBitmap.height() - divY.rem() - 1 : divY.rem();
		
		int i =  _srcBitmap.width() - divX.rem();
		
		int q = divX.quot();
		
		if (i >= count)
		{
			if (q % 2)
				blendOp->blendReversed(count, bitmap.pixelPointer(point),  _srcBitmap.constPixelPointer( _srcBitmap.width() - divX.rem() - count, imageY), cover);
			else
				blendOp->blend(count, bitmap.pixelPointer(point),  _srcBitmap.constPixelPointer(divX.rem(), imageY), cover);
			return;
		}
		
		if (q % 2)
			blendOp->blendReversed(i, bitmap.pixelPointer(point),  _srcBitmap.constPixelPointer(0, imageY), cover);
		else
			blendOp->blend(i, bitmap.pixelPointer(point),  _srcBitmap.constPixelPointer(divX.rem(), imageY), cover);
		
		q++;
		
		forever
		{
			if (count - i <  _srcBitmap.width()) break;
			
			if (q % 2)
				blendOp->blendReversed( _srcBitmap.width(), bitmap.pixelPointer(point + QPoint(i, 0)),  _srcBitmap.constPixelPointer(0, imageY), cover);
			else
				blendOp->blend( _srcBitmap.width(), bitmap.pixelPointer(point + QPoint(i, 0)),  _srcBitmap.constPixelPointer(0, imageY), cover);
			i += _srcBitmap.width();
			q++;
		}
		
		if (q % 2)
			blendOp->blendReversed(count - i, bitmap.pixelPointer(point + QPoint(i, 0)), _srcBitmap.constPixelPointer(_srcBitmap.width() - count + i, imageY), cover);
		else
			blendOp->blend(count - i, bitmap.pixelPointer(point + QPoint(i, 0)), _srcBitmap.constPixelPointer(0, imageY), cover);
		}
	
private:
	
	const MLBitmap<MLArgb> _srcBitmap;
	QPoint _offset;
};

#endif // MLIMAGERENDERER_H
