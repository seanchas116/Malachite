#ifndef MLIMAGERENDERERV2_H
#define MLIMAGERENDERERV2_H

#include <QPainterPath>
#include "agg_rasterizer_scanline_aa.h"
#include "agg_scanline_p.h"
#include "mlcurvesubdivision.h"
#include "mlimage.h"
#include "mlblendop.h"
#include "mlintdivision.h"

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
		
		QScopedArrayPointer<float> newCovers(new float[newCount]);
		
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

class MLColorFiller
{
public:
	MLColorFiller(const MLVec4F &argb, double opacity) :
		_argb(argb * opacity)
	{}
	
	void fill(const QPoint &pos, int count, MLVec4F *dst, float *covers, MLBlendOp *blendOp)
	{
		Q_UNUSED(pos);
		blendOp->blend(count, dst, _argb, covers);
	}
	
	void fill(const QPoint &pos, int count, MLVec4F *dst, float cover, MLBlendOp *blendOp)
	{
		Q_UNUSED(pos);
		blendOp->blend(count, dst, _argb * cover);
	}
	
private:
	MLVec4F _argb;
};

template <ML::SpreadType SpreadType>
class MLImageFiller
{
public:
	
	MLImageFiller(const MLArgbBitmap &bitmap, const QPoint &offset) :
		_srcBitmap(bitmap), _offset(offset) {}
	
	void fill(const QPoint &pos, int count, MLVec4F *dst, float *covers, MLBlendOp *blendOp)
	{
		fillTemplate<false>(pos, count, dst, covers, blendOp);
	}
	
	void fill(const QPoint &pos, int count, MLVec4F *dst, float cover, MLBlendOp *blendOp)
	{
		fillTemplate<true>(pos, count, dst, &cover, blendOp);
	}
	
private:
	
	template <bool CoverSingle>
	void fillTemplate(const QPoint &pos, int count, MLVec4F *dst, float *covers, MLBlendOp *blendOp)
	{
		switch (SpreadType)
		{
		case ML::SpreadTypePad:
			fillTemplatePad<CoverSingle>(pos, count, dst, covers, blendOp);
			return;
		case ML::SpreadTypeRepeat:
			fillTemplateRepeat<CoverSingle>(pos, count, dst, covers, blendOp);
			return;
		case ML::SpreadTypeReflective:
			fillTemplateReflective<CoverSingle>(pos, count, dst, covers, blendOp);
			return;
		}
	}
	
	template <bool CoverSingle>
	void fillTemplatePad(const QPoint &pos, int count, MLVec4F *dst, float *covers, MLBlendOp *blendOp)
	{
		QPoint srcPos = pos - _offset;
		int imageY = qBound(0, srcPos.y(), _srcBitmap.height() - 1);
		
		int i = 0;
		
		int left = -srcPos.x();
		
		if (left > 0)
		{
			if (CoverSingle)
				blendOp->blend(left, dst, _srcBitmap.pixel(0, imageY), *covers);
			else
				blendOp->blend(left, dst, _srcBitmap.pixel(0, imageY), covers);
			i += left;
		}
		
		int midStart = qMax(0, srcPos.x());
		int midEnd = qMin(_srcBitmap.width(), srcPos.x() + count);
		int mid = midEnd - midStart;
		
		if (mid > 0)
		{
			if (CoverSingle)
				blendOp->blend(mid, dst + i, _srcBitmap.constPixelPointer(midStart, imageY), *covers);
			else
				blendOp->blend(mid, dst + i, _srcBitmap.constPixelPointer(midStart, imageY), covers + i);
			i += mid;
		}
		
		int right = srcPos.x() + count - _srcBitmap.width();
		
		if (right > 0)
		{
			if (CoverSingle)
				blendOp->blend(mid, dst + i, _srcBitmap.pixel(_srcBitmap.width() - 1, imageY), *covers);
			else
				blendOp->blend(mid, dst + i, _srcBitmap.pixel(_srcBitmap.width() - 1, imageY), covers + i);
		}
	}
	
	template <bool CoverSingle>
	void fillTemplateRepeat(const QPoint &pos, int count, MLVec4F *dst, float *covers, MLBlendOp *blendOp)
	{
		QPoint srcPos = pos - _offset;
		MLIntDivision divX(srcPos.x(), _srcBitmap.width());
		MLIntDivision divY(srcPos.y(), _srcBitmap.height());
		
		int imageY = divY.rem();
		
		int i = _srcBitmap.width() - divX.rem();
		
		if (i >= count)
		{
			if (CoverSingle)
				blendOp->blend(count, dst, _srcBitmap.constPixelPointer(divX.rem(), imageY), *covers);
			else
				blendOp->blend(count, dst, _srcBitmap.constPixelPointer(divX.rem(), imageY), covers);
			return;
		}
		
		if (CoverSingle)
			blendOp->blend(i, dst, _srcBitmap.constPixelPointer(divX.rem(), imageY), *covers);
		else
			blendOp->blend(i, dst, _srcBitmap.constPixelPointer(divX.rem(), imageY), covers);
		
		forever
		{
			if (count - i < _srcBitmap.width()) break;
			
			if (CoverSingle)
				blendOp->blend(_srcBitmap.width(), dst + i, _srcBitmap.constPixelPointer(0, imageY), *covers);
			else
				blendOp->blend(_srcBitmap.width(), dst + i, _srcBitmap.constPixelPointer(0, imageY), covers + i);
			
			i += _srcBitmap.width();
		}
		
		if (CoverSingle)
			blendOp->blend(count - i, dst + i, _srcBitmap.constPixelPointer(0, imageY), *covers);
		else
			blendOp->blend(count - i, dst + i, _srcBitmap.constPixelPointer(0, imageY), covers + i);
	}
	
	template <bool CoverSingle>
	void fillTemplateReflective(const QPoint &pos, int count, MLVec4F *dst, float *covers, MLBlendOp *blendOp)
	{
		QPoint srcPos = pos - _offset;
		MLIntDivision divX(srcPos.x(), _srcBitmap.width());
		MLIntDivision divY(srcPos.y(), _srcBitmap.height());
		
		int imageY = divY.rem();
		
		int i = _srcBitmap.width() - divX.rem();
		
		if (i >= count)
		{
			if (CoverSingle)
				blendOp->blend(count, dst, _srcBitmap.constPixelPointer(divX.rem(), imageY), *covers);
			else
				blendOp->blend(count, dst, _srcBitmap.constPixelPointer(divX.rem(), imageY), covers);
			return;
		}
		
		if (CoverSingle)
			blendOp->blend(i, dst, _srcBitmap.constPixelPointer(divX.rem(), imageY), *covers);
		else
			blendOp->blend(i, dst, _srcBitmap.constPixelPointer(divX.rem(), imageY), covers);
		
		forever
		{
			if (count - i < _srcBitmap.width()) break;
			
			if (CoverSingle)
				blendOp->blend(_srcBitmap.width(), dst + i, _srcBitmap.constPixelPointer(0, imageY), *covers);
			else
				blendOp->blend(_srcBitmap.width(), dst + i, _srcBitmap.constPixelPointer(0, imageY), covers + i);
			
			i += _srcBitmap.width();
		}
		
		if (CoverSingle)
			blendOp->blend(count - i, dst + i, _srcBitmap.constPixelPointer(0, imageY), *covers);
		else
			blendOp->blend(count - i, dst + i, _srcBitmap.constPixelPointer(0, imageY), covers + i);
	}
	
	const MLBitmap<MLVec4F> _srcBitmap;
	QPoint _offset;
};

template <class Generator, bool TransformEnabled>
class MLFiller
{
public:
	MLFiller(Generator *generator, float opacity, const QTransform &worldTransform = QTransform()) :
		_generator(generator),
		_opacity(opacity),
		_transform(worldTransform)
	{}
	
	void fill(const QPoint &pos, int count, MLVec4F *dst, float *covers, MLBlendOp *blendOp)
	{
		if (_opacity != 1)
		{
			for (int i = 0; i < count; ++i)
				covers[i] *= _opacity;
		}
		
		QScopedArrayPointer<MLVec4F> fill(new MLVec4F[count]);
		
		MLVec2D centerPos(pos.x(), pos.y());
		centerPos += MLVec2D(0.5, 0.5);
		
		for (int i = 0; i < count; ++i)
		{
			fill[i] = _generator->at(TransformEnabled ? centerPos * _transform : centerPos);
			centerPos += MLVec2D(1, 0);
		}
		
		blendOp->blend(count, dst, fill.data(), covers);
	}
	
	void fill(const QPoint &pos, int count, MLVec4F *dst, float cover, MLBlendOp *blendOp)
	{
		cover *= _opacity;
		
		QScopedArrayPointer<MLVec4F> fill(new MLVec4F[count]);
		
		MLVec2D centerPos(pos.x(), pos.y());
		centerPos += MLVec2D(0.5, 0.5);
		
		for (int i = 0; i < count; ++i)
		{
			fill[i] = _generator->at(TransformEnabled ? centerPos * _transform : centerPos);
			centerPos += MLVec2D(1, 0);
		}
		
		blendOp->blend(count, dst, fill.data(), cover);
	}
	
private:
	Generator *_generator;
	float _opacity;
	QTransform _transform;
};

template <class Gradient, class Method, ML::SpreadType SpreadType>
class MLGradientGenerator
{
public:
	MLGradientGenerator(const Gradient *gradient, Method *method) :
		_gradient(gradient),
		_method(method)
	{}
	
	MLVec4F at(const MLVec2D &p)
	{
		return _gradient->at(actualPosition(_method->position(p)));
	}
	
private:
	
	float actualPosition(float x) const
	{
		switch (SpreadType)
		{
		default:
		case ML::SpreadTypePad:
			return qBound(0.f, x, 1.f);
		case ML::SpreadTypeRepeat:
			return x - floorf(x);
		case ML::SpreadTypeReflective:
		{
			float f = floorf(x);
			float r = x - f;
			return (int)f % 2 ? 1.f - r : r;
		}
		}
	}
	
	const Gradient *_gradient;
	Method *_method;
};

class MLLinearGradientMethod
{
public:
	
	MLLinearGradientMethod(const MLVec2D &start, const MLVec2D &end) :
		a(start),
		ab(end - start)
	{
		Q_ASSERT(start != end);
		ab2inv = 1.0 / mlSqLength(ab);
	}
	
	float position(const MLVec2D &p) const
	{
		return mlDot(p - a, ab) * ab2inv;
	}
	
private:
	MLVec2D a;
	MLVec2D ab;
	double ab2inv;
};

class MLRadialGradientMethod
{
public:
	
	MLRadialGradientMethod(const MLVec2D &center, double radius) :
		c(center)
	{
		Q_ASSERT(radius > 0);
		rinv = 1.0 / radius;
	}
	
	float position(const MLVec2D &p) const
	{
		MLVec2D d = p - c;
		return mlLength(d) * rinv;
	}
	
private:
	MLVec2D c;
	double rinv;
};

class MLFocalGradientMethod
{
public:
	
	MLFocalGradientMethod(const MLVec2D &center, double radius, const MLVec2D &focal) :
		o(center), f(focal), r(radius)
	{
		Q_ASSERT(r > 0);
		of = f - o;
		c = mlSqLength(of) - r * r;
		Q_ASSERT(c < 0);
	}
	
	float position(const MLVec2D &p) const
	{
		MLVec2D fp = p - f;
		double dot = mlDot(of, fp);
		double fp2 = mlSqLength(fp);
		
		return fp2 / (sqrt(dot * dot - fp2 * c) - dot); 
	}
	
private:
	MLVec2D o, f, of;
	double r, c;
};

template <ML::SpreadType SpreadType>
QPoint mlActualImagePos(const QPoint &p, const QSize &size)
{
	QPoint r;
	
	switch (SpreadType)
	{
	case ML::SpreadTypePad:
	{
		r.rx() = qBound(0, p.x(), size.width() - 1);
		r.ry() = qBound(0, p.y(), size.height() - 1);
		break;
	}
	case ML::SpreadTypeRepeat:
	{
		r.rx() = MLIntDivision(p.x(), size.width()).rem();
		r.ry() = MLIntDivision(p.y(), size.height()).rem();
		break;
	}
	case ML::SpreadTypeReflective:
	{
		MLIntDivision divX(p.x(), size.width());
		MLIntDivision divY(p.y(), size.height());
		r.rx() = divX.quot() % 2 ? size.width() - divX.rem() - 1 : divX.rem();
		r.rx() = divY.quot() % 2 ? size.height() - divY.rem() - 1 : divY.rem();
		break;
	}
	default:
		break;
	}
	
	return r;
}

template <class Source, ML::PixelFieldType SourceType, ML::SpreadType SpreadType>
class MLScalingGeneratorNearestNeighbor
{
public:
	MLScalingGeneratorNearestNeighbor(const Source *source) :
		_source(source)
	{}
	
	MLVec4F at(const MLVec2D &p)
	{
		if (SourceType == ML::PixelFieldImage)
			return _source->pixel(mlActualImagePos<SpreadType>(p.toQPoint(), _source->size()));
		else
			return _source->pixel(p.toQPoint());
	}
	
private:
	const Source *_source;
};

template <class Source, ML::PixelFieldType SourceType, ML::SpreadType SpreadType>
class MLScalingGeneratorBilinear
{
public:
	MLScalingGeneratorBilinear(const Source *source) :
		_source(source)
	{}
	
	MLVec4F at(const MLVec2D &p)
	{
		MLVec2D rp(round(p.x), round(p.y));
		MLVec2D d = p - rp + MLVec2D(0.5, 0.5);
		float dx = d.x;
		float dy = d.y;
		QPoint irp = rp.toQPoint();
		
		MLVec4F c11, c12, c21, c22;
		
		if (SourceType == ML::PixelFieldImage)
		{
			c11 = _source->pixel(mlActualImagePos<SpreadType>(irp + QPoint(-1, -1), _source->size()));
			c12 = _source->pixel(mlActualImagePos<SpreadType>(irp + QPoint(-1, 0), _source->size()));
			c21 = _source->pixel(mlActualImagePos<SpreadType>(irp + QPoint(0, -1), _source->size()));
			c22 = _source->pixel(mlActualImagePos<SpreadType>(irp, _source->size()));
		}
		else
			c11 = _source->pixel(irp + QPoint(-1, -1));
			c12 = _source->pixel(irp + QPoint(-1, 0));
			c21 = _source->pixel(irp + QPoint(0, -1));
			c22 = _source->pixel(irp);
		
		MLVec4F result;
		result.v = (c11 * (1.f - dx) + c21 * dx) * (1.f - dy) + (c12 * (1.f - dx) + c22 * dx) * dy;
		
		return result;
	}
	
private:
	const Source *_source;
};

template <class Source, ML::PixelFieldType SourceType, ML::SpreadType SpreadType, class WeightMethod>
class MLScalingGenerator2
{
public:
	MLScalingGenerator2(const Source *source) :
		_source(source)
	{}
	
	MLVec4F at(const MLVec2D &p)
	{
		reset(p);
		
		QPoint fp(round(p.x), round(p.y));
		
		if (SourceType != ML::PixelFieldImage ||  pointIsInSafeZone(fp))
		{
			if (SourceType == ML::PixelFieldImage)
				fp = mlActualImagePos<SpreadType>(fp, _source->size());
			
			addPixelEasy(fp + QPoint(-2, -2));
			addPixelEasy(fp + QPoint(1, -2));
			addPixelEasy(fp + QPoint(-2, 1));
			addPixelEasy(fp + QPoint(1, 1));
			
			addPixelEasy(fp + QPoint(-1, -2));
			addPixelEasy(fp + QPoint(0, -2));
			addPixelEasy(fp + QPoint(-2, -1));
			addPixelEasy(fp + QPoint(1, -1));
			addPixelEasy(fp + QPoint(-2, 0));
			addPixelEasy(fp + QPoint(1, 0));
			addPixelEasy(fp + QPoint(-1, 1));
			addPixelEasy(fp + QPoint(0, 1));
			
			addPixelEasy(fp + QPoint(-1, -1));
			addPixelEasy(fp + QPoint(0, -1));
			addPixelEasy(fp + QPoint(-1, 0));
			addPixelEasy(fp + QPoint(0, 0));
		}
		else
		{
			addPixel(fp + QPoint(-2, -2));
			addPixel(fp + QPoint(1, -2));
			addPixel(fp + QPoint(-2, 1));
			addPixel(fp + QPoint(1, 1));
			
			addPixel(fp + QPoint(-1, -2));
			addPixel(fp + QPoint(0, -2));
			addPixel(fp + QPoint(-2, -1));
			addPixel(fp + QPoint(1, -1));
			addPixel(fp + QPoint(-2, 0));
			addPixel(fp + QPoint(1, 0));
			addPixel(fp + QPoint(-1, 1));
			addPixel(fp + QPoint(0, 1));
			
			addPixel(fp + QPoint(-1, -1));
			addPixel(fp + QPoint(0, -1));
			addPixel(fp + QPoint(-1, 0));
			addPixel(fp + QPoint(0, 0));
		}
		
		MLVec4F result = _argb;
		
		if (_divisor == 0)
		{
			result = MLVec4F(0);
		}
		else
		{
			result /= _divisor;
			result = mlBound(0, result, 1);
		}
		
		return result;
	}
	
private:
	
	bool pointIsInSafeZone(const QPoint &p)
	{
		return 2 <= p.x() && p.x() < _source->width() - 2 && 2 <= p.y() && p.y() < _source->height() - 2;
	}
	
	void addPixel(const QPoint &p)
	{
		float w = WeightMethod::weight(MLVec2D(p) + MLVec2D(0.5, 0.5) - _p);
		_divisor += w;
		_argb += _source->pixel(mlActualImagePos<SpreadType>(p, _source->size())) * w;
	}
	
	void addPixelEasy(const QPoint &p)
	{
		float w = WeightMethod::weight(MLVec2D(p) + MLVec2D(0.5, 0.5) - _p);
		_divisor += w;
		_argb += _source->pixel(p) * w;
	}
	
	void reset()
	{
		_divisor = 0;
		_argb = MLVec4F(0);
	}
	
	void reset(const MLVec2D &p)
	{
		reset();
		_p = p;
	}

	MLVec2D _p;
	float _divisor;
	MLVec4F _argb;
	const Source *_source;
};

class MLScalingWeightMethodBicubic
{
public:
	static double weight(const MLVec2D &d)
	{
		double dx = fabs(d.x);
		double dy = fabs(d.y);
		
		double wx = dx <= 1.0 ? f01(dx) : f12(dx);
		double wy = dy <= 1.0 ? f01(dy) : f12(dy);
		return wx * wy;
	}
	
private:
	
	static double f01(double x)
	{
		//return x*x*x - 2.0*x*x + 1.0;
		return x*x * (x - 2.0) + 1.0;
	}
	
	static double f12(double x)
	{
		//return -x*x*x + 5.0*x*x - 8*x + 4;
		return x * (x * (-x + 5.0) - 8.0) + 4.0;
	}
};

class MLScalingWeightMethodLanczos2
{
public:
	static double weight(const MLVec2D &d)
	{
		MLVec2D pd = M_PI * d;
		MLVec2D hpd = 0.5 * M_PI * d;
		MLVec2D dd = d * d;
		
		return sin(pd.x) * sin(hpd .x) * sin(pd.y) * sin(hpd.y) / (0.25 * M_PI * M_PI * M_PI * M_PI * dd.x * dd.y);
	}
};

class MLScalingWeightMethodLanczos2Hypot
{
public:
	static double weight(const MLVec2D &d)
	{
		double dist = mlLength(d);
		return sin(M_PI * dist) * sin(0.5 * M_PI * dist) / (0.5 * M_PI * M_PI * dist * dist);
		
	}
};

#endif // MLIMAGERENDERERV2_H
