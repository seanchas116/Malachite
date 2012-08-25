#ifndef MLSCALINGGENERATOR_H
#define MLSCALINGGENERATOR_H

#include "mlvector.h"
#include "mldivision.h"

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
		r.rx() = (divX.quot() % 2) ? (size.width() - divX.rem() - 1) : divX.rem();
		r.ry() = (divY.quot() % 2) ? (size.height() - divY.rem() - 1) : divY.rem();
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


#endif // MLSCALINGGENERATOR_H
