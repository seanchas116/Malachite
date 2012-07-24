#ifndef MLFILLGENERATOR_H
#define MLFILLGENERATOR_H

#include "mlpixelgenerator.h"
#include "mlbitmap.h"
#include "mlintdivision.h"

class MLFillGenerator : public MLPixelGenerator
{
public:
	MLFillGenerator() :
		MLPixelGenerator()
	{}
	
	void setSpreadType(MLGlobal::SpreadType type) { _spreadType = type; }
	MLGlobal::SpreadType spreadType() const { return _spreadType; }
	
private:
	MLGlobal::SpreadType _spreadType;
};

template <class Gradient>
class MLGradientFillGenerator : public MLFillGenerator
{
public:
	
	MLGradientFillGenerator(const Gradient *gradient) :
		MLFillGenerator(),
		_gradient(gradient)
	{}
	
	MLArgb at(const QPointF &point)
	{
		return _gradient->at(actualPosition(position(point)));
	}
	
protected:
	
	virtual double position(const QPointF &p) const = 0;
	
private:
	
	double actualPosition(double x) const
	{
		switch (spreadType())
		{
		default:
		case MLGlobal::SpreadTypePad:
			return qBound(0.0, x, 1.0);
		case MLGlobal::SpreadTypeRepeat:
			return x - floor(x);
		case MLGlobal::SpreadTypeReflective:
		{
			double f = floor(x);
			double r = x - f;
			return (int)f % 2 ? 1.0f - r : r;
		}
		}
	}
	
	const Gradient *_gradient;
};

/*
class MLLinearGradientMethod
{
public:
	
	MLLinearGradientMethod(const QPointF &start, const QPointF &end) :
		_a(start),
		_ab(end - start)
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
		_c(center),
		_r(radius)
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

*/

template <class Gradient>
class MLLinearGradientFillGenerator : public MLGradientFillGenerator<Gradient>
{
public:
	
	MLLinearGradientFillGenerator(const Gradient *gradient, const QPointF &start, const QPointF &end) :
		MLGradientFillGenerator<Gradient>(gradient),
		_a(start),
		_ab(end - start)
	{
		_ab2 = _ab.x() * _ab.x() + _ab.y() * _ab.y();
		Q_ASSERT(_ab2 != 0);
	}
	
protected:
	
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

template <class Gradient>
class MLRadialGradientFillGenerator : public MLGradientFillGenerator<Gradient>
{
public:
	
	MLRadialGradientFillGenerator(const Gradient *gradient, const QPointF &center, double radius) :
		MLGradientFillGenerator<Gradient>(gradient),
		_c(center),
		_r(radius)
	{
		Q_ASSERT(_r > 0);
	}
	
protected:
	
	double position(const QPointF &p) const
	{
		QPointF d = p - _c;
		return hypot(d.x(), d.y()) / _r;
	}
	
private:
	QPointF _c;
	double _r;
};

template <class Gradient>
class MLFocalGradientFillGenerator : public MLGradientFillGenerator<Gradient>
{
public:
	
	MLFocalGradientFillGenerator(const Gradient *gradient, const QPointF &center, double radius, const QPointF &focal) :
		MLGradientFillGenerator<Gradient>(gradient),
		o(center), f(focal), r(radius)
	{
		Q_ASSERT(r > 0);
		of = f - o;
		c = of.x() * of.x() + of.y() * of.y() - r * r;
		Q_ASSERT(c < 0);
	}
	
protected:
	
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

inline QPoint mlActualPoint(const QPoint &p, const QSize &bitmapSize, MLGlobal::SpreadType spreadType)
{
	QPoint r;
	
	switch (spreadType)
	{
	case MLGlobal::SpreadTypePad:
	{
		r.rx() = qBound(0, p.x(), bitmapSize.width());
		r.ry() = qBound(0, p.y(), bitmapSize.height());
		break;
	}
	case MLGlobal::SpreadTypeRepeat:
	{
		r.rx() = MLIntDivision(p.x(), bitmapSize.width()).rem();
		r.ry() = MLIntDivision(p.y(), bitmapSize.height()).rem();
		break;
	}
	case MLGlobal::SpreadTypeReflective:
	{
		MLIntDivision divX(p.x(), bitmapSize.width());
		MLIntDivision divY(p.y(), bitmapSize.height());
		r.rx() = divX.quot() % 2 ? bitmapSize.width() - divX.rem() - 1 : divX.rem();
		r.rx() = divY.quot() % 2 ? bitmapSize.height() - divY.rem() - 1 : divY.rem();
		break;
	}
	default:
		break;
	}
	
	return r;
}

template <class Source, bool SourceIsBound>
class MLScalingFilter : public MLFillGenerator
{
public:
	
	MLScalingFilter(const Source &source) : 
		MLFillGenerator(),
		_source(source)
	{}
	
protected:
	
	QPoint actualPoint(const QPoint &p) const
	{
		QPoint r;
		
		switch (spreadType())
		{
		case MLGlobal::SpreadTypePad:
		{
			r.rx() = qBound(0, p.x(), _source.width() - 1);
			r.ry() = qBound(0, p.y(), _source.height() - 1);
			break;
		}
		case MLGlobal::SpreadTypeRepeat:
		{
			r.rx() = MLIntDivision(p.x(), _source.width()).rem();
			r.ry() = MLIntDivision(p.y(), _source.height()).rem();
			break;
		}
		case MLGlobal::SpreadTypeReflective:
		{
			MLIntDivision divX(p.x(), _source.width());
			MLIntDivision divY(p.y(), _source.height());
			r.rx() = divX.quot() % 2 ? _source.width() - divX.rem() - 1 : divX.rem();
			r.rx() = divY.quot() % 2 ? _source.height() - divY.rem() - 1 : divY.rem();
			break;
		}
		default:
			break;
		}
		
		return r;
	}
	
protected:
	
	const Source source() const { return _source; }
	
private:
	const Source _source;
};

template <class Source, bool SourceIsBound>
class MLScalingFilterNearestNeighbor : public MLScalingFilter<Source, SourceIsBound>
{
public:
	
	MLScalingFilterNearestNeighbor(const Source &source) :
		MLScalingFilter<Source, SourceIsBound>(source) {}
	
	MLArgb at(const QPointF &p)
	{
		return SourceIsBound ? MLScalingFilter<Source, SourceIsBound>::source().pixel(MLScalingFilter<Source, SourceIsBound>::actualPoint(p.toPoint())) : MLScalingFilter<Source, SourceIsBound>::source().pixel(p.toPoint());
	}
};

template <class Source, bool SourceIsBound>
class MLScalingFilterBilinear : public MLScalingFilter<Source, SourceIsBound>
{
public:
	
	MLScalingFilterBilinear(const Source &source) : 
		MLScalingFilter<Source, SourceIsBound>(source)
	{}
	
	MLArgb at(const QPointF &p)
	{
		QPointF rp = mlPointRound(p);
		QPointF d = p - rp + QPointF(0.5, 0.5);
		float dx = d.x();
		float dy = d.y();
		QPoint irp = rp.toPoint();
		
		MLArgb c11 = source().pixel(actualPoint(irp + QPoint(-1, -1)));
		MLArgb c12 = source().pixel(actualPoint(irp + QPoint(-1, 0)));
		MLArgb c21 = source().pixel(actualPoint(irp + QPoint(0, -1)));
		MLArgb c22 = source().pixel(actualPoint(irp));
		
		MLArgb result;
		result.v = (c11.v * (1.f - dx) + c21.v * dx) * (1.f - dy) + (c12.v * (1.f - dx) + c22.v * dx) * dy;
		
		return result;
	}
};

template <class Source, bool SourceIsBound>
class MLScalingFilter2 : public MLScalingFilter<Source, SourceIsBound>
{
public:
	
	MLScalingFilter2(const Source &source) :
		MLScalingFilter<Source, SourceIsBound>(source)
	{
		reset();
	}
	
	MLArgb at(const QPointF &p)
	{
		reset(p);
		
		QPoint fp = mlPointRound(p).toPoint();
		
		if (!SourceIsBound ||  pointIsInSafeZone(fp))
		{
			if (SourceIsBound)
				fp = MLScalingFilter<Source, SourceIsBound>::actualPoint(fp);
			
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
		
		MLArgb result = _argb;
		
		if (_divisor == 0)
		{
			result.v = MLSimdF4(0);
		}
		else
		{
			result.v /= _divisor;
			result.clamp();
		}
		
		return result;
	}
	
protected:
	
	virtual double weight(const QPointF &delta) const = 0;
	
private:
	
	bool pointIsInSafeZone(const QPoint &p)
	{
		return 2 <= p.x() && p.x() < MLScalingFilter<Source, SourceIsBound>::source().width() - 2 && 2 <= p.y() && p.y() < MLScalingFilter<Source, SourceIsBound>::source().height() - 2;
	}
	
	void addPixel(const QPoint &p)
	{
		float w = weight(p + QPointF(0.5, 0.5) - _p);
		_divisor += w;
		_argb.v += MLScalingFilter<Source, SourceIsBound>::source().pixel(MLScalingFilter<Source, SourceIsBound>::actualPoint(p)).v * w;
	}
	
	void addPixelEasy(const QPoint &p)
	{
		float w = weight(p + QPointF(0.5, 0.5) - _p);
		_divisor += w;
		_argb.v += MLScalingFilter<Source, SourceIsBound>::source().pixel(p).v * w;
	}
	
	void reset()
	{
		_divisor = 0;
		_argb.v = MLSimdF4(0);
	}
	
	void reset(const QPointF &p)
	{
		reset();
		_p = p;
	}

	QPointF _p;
	float _divisor;
	MLArgb _argb;
	const MLBitmap<MLArgb> *_bitmap;
};

template <class Source, bool SourceIsBound>
class MLScalingFilterBicubic : public MLScalingFilter2<Source, SourceIsBound>
{
public:
	
	MLScalingFilterBicubic(const Source &source) :
		MLScalingFilter2<Source, SourceIsBound>(source)
	{}
	
	double weight(const QPointF &delta) const
	{
		double dx = fabs(delta.x());
		double dy = fabs(delta.y());
		
		double wx = dx <= 1.0 ? f01(dx) : f12(dx);
		double wy = dy <= 1.0 ? f01(dy) : f12(dy);
		return wx * wy;
	}
	
private:
	
	static double f01(double x)
	{
		return x*x*x - 2.0*x*x + 1.0;
	}
	
	static double f12(double x)
	{
		return -x*x*x + 5.0*x*x - 8*x + 4;
	}
};

template <class Source, bool SourceIsBound>
class MLScalingFilterSpline16 : public MLScalingFilter2<Source, SourceIsBound>
{
public:
	
	MLScalingFilterSpline16(const Source &source) :
		MLScalingFilter2<Source, SourceIsBound>(source)
	{}
	
	double weight(const QPointF &delta) const
	{
		double dx = fabs(delta.x());
		double dy = fabs(delta.y());
		double wx = dx <= 1.0 ? f01(dx) : f12(dx);
		double wy = dy <= 1.0 ? f01(dy) : f12(dy);
		return wx * wy;
	}
	
private:
	
	static double f01(double x)
	{
		return x*x*x - (9.0/5.0)*x*x - (1.0/5.0)*x + 1.0;
	}
	
	static double f12(double x)
	{
		return (-1.0/3.0)*x*x*x + (9.0/5.0)*x*x - (46.0/15.0)*x + 8.0/5.0;
	}
};

template <class Source, bool SourceIsBound>
class MLScalingFilterLanczos2 : public MLScalingFilter2<Source, SourceIsBound>
{
public:
	
	MLScalingFilterLanczos2(const Source &source) :
		MLScalingFilter2<Source, SourceIsBound>(source)
	{}
	
	double weight(const QPointF &delta) const
	{
		return sin(M_PI * delta.x()) * sin(0.5 * M_PI * delta.x()) * sin(M_PI * delta.y()) * sin(0.5 * M_PI * delta.y())
				/ (0.25 * M_PI * M_PI * M_PI * M_PI * delta.x() * delta.x() * delta.y() * delta.y());
	}
};

template <class Source, bool SourceIsBound>
class MLScalingFilterLanczos2Hypot : public MLScalingFilter2<Source, SourceIsBound>
{
public:
	
	MLScalingFilterLanczos2Hypot(const Source &source) :
		MLScalingFilter2<Source, SourceIsBound>(source)
	{}
	
	double weight(const QPointF &delta) const
	{
		double dist = hypot(delta.x(), delta.y());
		return sin(M_PI * dist) * sin(0.5 * M_PI * dist) / (0.5 * M_PI * M_PI * dist * dist);
	}
};

#endif // MLFILLGENERATOR_H
