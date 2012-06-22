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
#include "mlscalingfilter.h"
#include "agg_array.h"

namespace agg
{

//-------------------------------------------------------------curve4_points
struct curve4_points
{
    double cp[8];
    curve4_points() {}
    curve4_points(double x1, double y1,
                  double x2, double y2,
                  double x3, double y3,
                  double x4, double y4)
    {
        cp[0] = x1; cp[1] = y1; cp[2] = x2; cp[3] = y2;
        cp[4] = x3; cp[5] = y3; cp[6] = x4; cp[7] = y4;
    }
    void init(double x1, double y1,
              double x2, double y2,
              double x3, double y3,
              double x4, double y4)
    {
        cp[0] = x1; cp[1] = y1; cp[2] = x2; cp[3] = y2;
        cp[4] = x3; cp[5] = y3; cp[6] = x4; cp[7] = y4;
    }
    double  operator [] (unsigned i) const { return cp[i]; }
    double& operator [] (unsigned i)       { return cp[i]; }
};


//-------------------------------------------------------------curve4_div
class curve4_div
{
public:
    curve4_div() : 
        m_approximation_scale(1.0),
        m_angle_tolerance(0.0),
        m_cusp_limit(0.0),
        m_count(0)
    {}

    curve4_div(double x1, double y1, 
               double x2, double y2, 
               double x3, double y3,
               double x4, double y4) :
        m_approximation_scale(1.0),
        m_angle_tolerance(0.0),
        m_cusp_limit(0.0),
        m_count(0)
    { 
        init(x1, y1, x2, y2, x3, y3, x4, y4);
    }

    curve4_div(const curve4_points& cp) :
        m_approximation_scale(1.0),
        m_angle_tolerance(0.0),
        m_count(0)
    { 
        init(cp[0], cp[1], cp[2], cp[3], cp[4], cp[5], cp[6], cp[7]);
    }

    void reset() { m_points.remove_all(); m_count = 0; }
    void init(double x1, double y1, 
              double x2, double y2, 
              double x3, double y3,
              double x4, double y4);

    void init(const curve4_points& cp)
    {
        init(cp[0], cp[1], cp[2], cp[3], cp[4], cp[5], cp[6], cp[7]);
    }

    void approximation_scale(double s) { m_approximation_scale = s; }
    double approximation_scale() const { return m_approximation_scale;  }

    void angle_tolerance(double a) { m_angle_tolerance = a; }
    double angle_tolerance() const { return m_angle_tolerance;  }

    void cusp_limit(double v) 
    { 
        m_cusp_limit = (v == 0.0) ? 0.0 : pi - v; 
    }

    double cusp_limit() const 
    { 
        return (m_cusp_limit == 0.0) ? 0.0 : pi - m_cusp_limit; 
    }

    void rewind(unsigned)
    {
        m_count = 0;
    }

    unsigned vertex(double* x, double* y)
    {
        if(m_count >= m_points.size()) return path_cmd_stop;
        const point_d& p = m_points[m_count++];
        *x = p.x;
        *y = p.y;
        return (m_count == 1) ? path_cmd_move_to : path_cmd_line_to;
    }

private:
    void bezier(double x1, double y1, 
                double x2, double y2, 
                double x3, double y3, 
                double x4, double y4);

    void recursive_bezier(double x1, double y1, 
                          double x2, double y2, 
                          double x3, double y3, 
                          double x4, double y4,
                          unsigned level);

    double               m_approximation_scale;
    double               m_distance_tolerance_square;
    double               m_angle_tolerance;
    double               m_cusp_limit;
    unsigned             m_count;
    pod_bvector<point_d> m_points;
};

}

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
		
		_filler->blend(_bitmap, _blendOp, QPoint(start, y), newCount, newCovers);
		
		delete[] newCovers;
	}
	
	void blendRasterizerLine(int x, int y, int count, uint8_t cover)
	{
		if (y < _bitmap.rect().top() || _bitmap.rect().bottom() < y)
			return;
		
		int start = qMax(x, _bitmap.rect().left());
		int end = qMin(x + count, _bitmap.rect().left() + _bitmap.rect().width());
		unsigned newCount = end - start;
		
		_filler->blend(_bitmap, _blendOp, QPoint(start, y), newCount, (float)cover / 255.0f);
	}
	
private:
	MLBitmap<MLArgb> _bitmap;
	MLBlendOp *_blendOp;
	Filler *_filler;
};


class MLColorFiller
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

/*
class MLGradientMethod
{
public:
	MLGradientMethod() {}
	virtual ~MLGradientMethod() {}
	
	virtual double position(const QPointF &p) const = 0;
};
*/

class MLLinearGradientMethod
{
public:
	MLLinearGradientMethod(const QPointF &a, const QPointF &b) :
		_a(a), _ab(b-a)
	{
		_ab2 = _ab.x() * _ab.x() + _ab.y() * _ab.y();
		Q_ASSERT(_ab2 != 0);
	}
	
	double position(const QPointF &p) const
	{
		QPointF ap = p -_a;
		return (ap.x() * _ab.x() + ap.y() * _ab.y()) / _ab2;
	}
	
private:
	QPointF _a;
	QPointF _ab;
	double _ab2;
};

class MLRadialGradientMethod
{
public:
	MLRadialGradientMethod(const QPointF &center, double radius) :
		_c(center), _r(radius)
	{
		Q_ASSERT(_r > 0);
	}
	
	double position(const QPointF &p) const
	{
		QPointF d = p - _c;
		return hypot(d.x(), d.y()) / _r;
	}
	
private:
	QPointF _c;
	double _r;
};

class MLFocalGradientMethod
{
public:
	MLFocalGradientMethod(const QPointF &center, double radius, const QPointF &focal) :
		o(center), f(focal), r(radius)
	{
		Q_ASSERT(r > 0);
		of = f - o;
		c = of.x() * of.x() + of.y() * of.y() - r * r;
		Q_ASSERT(c < 0);
	}
	
	double position(const QPointF &p) const
	{
		QPointF fp = p - f;
		double dot = of.x() * fp.x() + of.y() * fp.y();
		double fp2 = fp.x() * fp.x() + fp.y() * fp.y();
		
		return fp2 / (sqrt(dot * dot - fp2 * c) - dot); 
	}
	
private:
	QPointF o, f, of;
	double r, c;
};


inline float mlActualGradientPosition(float position, MLGlobal::SpreadType spreadType)
{
	switch (spreadType)
	{
	default:
	case MLGlobal::SpreadTypePad:
		return qBound(0.0f, position, 1.0f);
	case MLGlobal::SpreadTypeRepeat:
		return position - floorf(position);
	case MLGlobal::SpreadTypeReflective:
	{
		float f = floorf(position);
		float r = position - f;
		return (int)f % 2 ? 1.0f - r : r;
	}
	}
}

template <class Gradient, class Method, MLGlobal::SpreadType SpreadType>
class MLGradientFiller
{
public:
	
	MLGradientFiller(const Gradient *gradient, const Method *gradientMethod) :
		_gradient(gradient), _method(gradientMethod) {}
	
	void blend(MLBitmap<MLArgb> &bitmap, MLBlendOp *blendOp, const QPoint &point, int count, float *covers)
	{
		MLArgb *fill = new MLArgb[count];
		
		for (int i = 0; i < count; ++i)
		{
			QPointF p((point.x() + i) + 0.5, point.y() + 0.5);
			fill[i] = _gradient->at(mlActualGradientPosition(_method->position(p), SpreadType));
		}
		
		blendOp->blend(count, bitmap.pixelPointer(point), fill, covers);
		
		delete fill;
	}
	
	void blend(MLBitmap<MLArgb> &bitmap, MLBlendOp *blendOp, const QPoint &point, int count, float cover)
	{
		MLArgb *fill = new MLArgb[count];
		
		for (int i = 0; i < count; ++i)
		{
			QPointF p((point.x() + i) + 0.5, point.y() + 0.5);
			fill[i] = _gradient->at(mlActualGradientPosition(_method->position(p), SpreadType));
			fill[i].v *= cover;
		}
		
		blendOp->blend(count, bitmap.pixelPointer(point), fill);
		
		delete fill;
	}
	
private:
	
	const Gradient *_gradient;
	const Method *_method;
};

template <class Gradient, class Method, MLGlobal::SpreadType SpreadType>
class MLTransformedGradientFiller
{
public:
	
	MLTransformedGradientFiller(const Gradient *gradient, const Method *gradientMethod, const QTransform &worldTransform) :
		_gradient(gradient), _method(gradientMethod), _transform(worldTransform) {}
	
	void blend(MLBitmap<MLArgb> &bitmap, MLBlendOp *blendOp, const QPoint &point, int count, float *covers)
	{
		MLArgb *fill = new MLArgb[count];
		
		for (int i = 0; i < count; ++i)
		{
			QPointF p((point.x() + i) + 0.5, point.y() + 0.5);
			fill[i] = _gradient->at(mlActualGradientPosition(_method->position(p * _transform), SpreadType));
		}
		
		blendOp->blend(count, bitmap.pixelPointer(point), fill, covers);
		
		delete fill;
	}
	
	void blend(MLBitmap<MLArgb> &bitmap, MLBlendOp *blendOp, const QPoint &point, int count, float cover)
	{
		MLArgb *fill = new MLArgb[count];
		
		for (int i = 0; i < count; ++i)
		{
			QPointF p((point.x() + i) + 0.5, point.y() + 0.5);
			fill[i] = _gradient->at(mlActualGradientPosition(_method->position(p * _transform), SpreadType));
			fill[i].v *= cover;
		}
		
		blendOp->blend(count, bitmap.pixelPointer(point), fill);
		
		delete fill;
	}
	
private:
	
	const Gradient *_gradient;
	const Method *_method;
	QTransform _transform;
};

class MLPadImageFiller
{
public:
	MLPadImageFiller(const MLImage &image, const QPoint &offset) :
		_srcBitmap(image.constBitmap()), _offset(offset) {}
	
	void blend(MLBitmap<MLArgb> &bitmap, MLBlendOp *blendOp, const QPoint &point, int count, float *covers)
	{
		QPoint srcPoint = point - _offset;
		int imageY = qBound(0, srcPoint.y(), _srcBitmap.height() - 1);
		
		int i = 0;
		
		int left = qMin(0 - srcPoint.x(), count);
		
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
			blendOp->blend(mid, bitmap.pixelPointer(point + QPoint(i, 0)), _srcBitmap.pixel(midStart, imageY), covers + i);
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
		
		int left = qMin(0 - srcPoint.x(), count);
		
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
			blendOp->blend(mid, bitmap.pixelPointer(point + QPoint(i, 0)), _srcBitmap.pixel(midStart, imageY), cover);
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

class MLRepeatImageFiller
{
public:
	MLRepeatImageFiller(const MLImage &image, const QPoint &offset) :
		_srcBitmap(image.constBitmap()), _offset(offset) {}
	
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

class MLReflectiveImageFiller
{
public:
	MLReflectiveImageFiller(const MLImage &image, const QPoint &offset) :
		_srcBitmap(image.constBitmap()), _offset(offset) {}
	
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

template <MLGlobal::SpreadType SpreadType>
class MLTransformedImageFiller
{
public:
	
	MLTransformedImageFiller(const MLImage &image, const QTransform &worldTransform) :
		_srcBitmap(image.constBitmap()), _transform(worldTransform) {}
	
	void blend(MLBitmap<MLArgb> &bitmap, MLBlendOp *blendOp, const QPoint &point, int count, float *covers)
	{
		MLArgb *fill= new MLArgb[count];
		
		MLScalingFilterLanczos2Hypot<SpreadType> scaling(&_srcBitmap);
		
		for (int i = 0; i < count; ++i)
		{
			fill[i] = scaling.at((point + QPoint(i, 0) + QPointF(0.5, 0.5)) * _transform);
		}
		
		blendOp->blend(count, bitmap.pixelPointer(point), fill, covers);
		
		delete fill;
	}
	
	void blend(MLBitmap<MLArgb> &bitmap, MLBlendOp *blendOp, const QPoint &point, int count, float cover)
	{
		MLArgb *fill= new MLArgb[count];
		
		MLScalingFilterLanczos2Hypot<SpreadType> scaling(&_srcBitmap);
		
		for (int i = 0; i < count; ++i)
		{
			fill[i] = scaling.at((point + QPoint(i, 0) + QPointF(0.5, 0.5)) * _transform);
			fill[i].v *= cover;
		}
		
		blendOp->blend(count, bitmap.pixelPointer(point), fill);
		
		delete fill;
	}
	
private:
	
	const MLBitmap<MLArgb> _srcBitmap;
	QTransform _transform;
};


#endif // MLIMAGERENDERER_H
