#ifndef MLSCALINGGENERATOR_H
#define MLSCALINGGENERATOR_H

#include "../vec2d.h"
#include "../pixel.h"
#include "../division.h"

namespace Malachite
{

template <Malachite::SpreadType T_SpreadType>
QPoint actualImagePos(const QPoint &p, const QSize &size)
{
	QPoint r;
	
	switch (T_SpreadType)
	{
	case Malachite::SpreadTypePad:
	{
		r.rx() = qBound(0, p.x(), size.width() - 1);
		r.ry() = qBound(0, p.y(), size.height() - 1);
		break;
	}
	case Malachite::SpreadTypeRepeat:
	{
		r.rx() = IntDivision(p.x(), size.width()).rem();
		r.ry() = IntDivision(p.y(), size.height()).rem();
		break;
	}
	case Malachite::SpreadTypeReflective:
	{
		IntDivision divX(p.x(), size.width());
		IntDivision divY(p.y(), size.height());
		r.rx() = (divX.quot() % 2) ? (size.width() - divX.rem() - 1) : divX.rem();
		r.ry() = (divY.quot() % 2) ? (size.height() - divY.rem() - 1) : divY.rem();
		break;
	}
	default:
		break;
	}
	
	return r;
}

template <class Source, Malachite::PixelFieldType T_SourceType, Malachite::SpreadType T_SpreadType>
class ScalingGeneratorNearestNeighbor
{
public:
	ScalingGeneratorNearestNeighbor(const Source *source) :
		_source(source)
	{}
	
	Pixel at(const Vec2D &p)
	{
		if (T_SourceType == Malachite::PixelFieldImage)
			return _source->pixel(actualImagePos<T_SpreadType>(p.toQPoint(), _source->size()));
		else
			return _source->pixel(p.toQPoint());
	}
	
private:
	const Source *_source;
};

template <class Source, Malachite::PixelFieldType T_SourceType, Malachite::SpreadType T_SpreadType>
class ScalingGeneratorBilinear
{
public:
	ScalingGeneratorBilinear(const Source *source) :
		_source(source)
	{}
	
	Pixel at(const Vec2D &p)
	{
		Vec2D rp(round(p.x()), round(p.y()));
		Vec2D d = p - rp + Vec2D(0.5, 0.5);
		float dx = d.x();
		float dy = d.y();
		QPoint irp = rp.toQPoint();
		
		Pixel c11, c12, c21, c22;
		
		if (T_SourceType == Malachite::PixelFieldImage)
		{
			c11 = _source->pixel(actualImagePos<T_SpreadType>(irp + QPoint(-1, -1), _source->size()));
			c12 = _source->pixel(actualImagePos<T_SpreadType>(irp + QPoint(-1, 0), _source->size()));
			c21 = _source->pixel(actualImagePos<T_SpreadType>(irp + QPoint(0, -1), _source->size()));
			c22 = _source->pixel(actualImagePos<T_SpreadType>(irp, _source->size()));
		}
		else
			c11 = _source->pixel(irp + QPoint(-1, -1));
			c12 = _source->pixel(irp + QPoint(-1, 0));
			c21 = _source->pixel(irp + QPoint(0, -1));
			c22 = _source->pixel(irp);
		
		Pixel result;
		result.rv() = (c11.v() * (1.f - dx) + c21.v() * dx) * (1.f - dy) + (c12.v() * (1.f - dx) + c22.v() * dx) * dy;
		
		return result;
	}
	
private:
	const Source *_source;
};

template <class T_Source, Malachite::PixelFieldType T_SourceType, Malachite::SpreadType T_SpreadType, class T_WeightMethod>
class ScalingGenerator2
{
public:
	ScalingGenerator2(const T_Source *source) :
		_source(source)
	{}
	
	Pixel at(const Vec2D &p)
	{
		reset(p);
		
		QPoint fp(round(p.x()), round(p.y()));
		
		if (T_SourceType != Malachite::PixelFieldImage ||  pointIsInSafeZone(fp))
		{
			if (T_SourceType == Malachite::PixelFieldImage)
				fp = actualImagePos<T_SpreadType>(fp, _source->size());
			
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
		
		Pixel result = _argb;
		
		if (_divisor == 0)
		{
			result = Pixel(0);
		}
		else
		{
			result.rv() /= _divisor;
			result.rv() = result.v().bound(PixelVec(0), PixelVec(1));
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
		float w = T_WeightMethod::weight(Vec2D(p) + Vec2D(0.5, 0.5) - _p);
		_divisor += w;
		_argb.rv() += _source->pixel(actualImagePos<T_SpreadType>(p, _source->size())).v() * w;
	}
	
	void addPixelEasy(const QPoint &p)
	{
		float w = T_WeightMethod::weight(Vec2D(p) + Vec2D(0.5, 0.5) - _p);
		_divisor += w;
		_argb.rv() += _source->pixel(p).v() * w;
	}
	
	void reset()
	{
		_divisor = 0;
		_argb = Pixel(0);
	}
	
	void reset(const Vec2D &p)
	{
		reset();
		_p = p;
	}

	Vec2D _p;
	float _divisor;
	Pixel _argb;
	const T_Source *_source;
};

class ScalingWeightMethodBicubic
{
public:
	static double weight(const Vec2D &d)
	{
		double dx = fabs(d.x());
		double dy = fabs(d.y());
		
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

class ScalingWeightMethodLanczos2
{
public:
	static double weight(const Vec2D &d)
	{
		Vec2D pd = M_PI * d;
		Vec2D hpd = 0.5 * M_PI * d;
		Vec2D dd = d * d;
		
		return sin(pd.x()) * sin(hpd .x()) * sin(pd.y()) * sin(hpd.y()) / (0.25 * M_PI * M_PI * M_PI * M_PI * dd.x() * dd.y());
	}
};

class ScalingWeightMethodLanczos2Hypot
{
public:
	static double weight(const Vec2D &d)
	{
		double dist = d.length();
		return sin(M_PI * dist) * sin(0.5 * M_PI * dist) / (0.5 * M_PI * M_PI * dist * dist);
	}
};

}

#endif // MLSCALINGGENERATOR_H
