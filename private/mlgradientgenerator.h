#ifndef MLGRADIENTGENERATOR_H
#define MLGRADIENTGENERATOR_H

#include "mlvector.h"

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

/*
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
};*/

class MLRadialGradientMethod
{
public:
	
	MLRadialGradientMethod(const MLVec2D &center, const MLVec2D &radius) :
		c(center)
	{
		Q_ASSERT(radius.x > 0 && radius.y > 0);
		rinv = 1.0 / radius;
	}
	
	float position(const MLVec2D &p) const
	{
		MLVec2D d = p - c;
		return mlLength(d * rinv);
	}
	
private:
	MLVec2D c, rinv;
};

/*
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
*/

class MLFocalGradientMethod
{
public:
	
	MLFocalGradientMethod(const MLVec2D &center, const MLVec2D &radius, const MLVec2D &focal) :
		o(center)
	{
		Q_ASSERT(radius.x > 0 && radius.y > 0);
		rinv = 1 / radius;
		
		of = (focal - center) * rinv;
		c = mlSqLength(of) - 1;
		Q_ASSERT(c < 0);
	}
	
	float position(const MLVec2D &p) const
	{
		MLVec2D op = (p - o) * rinv;
		MLVec2D fp = op - of;
		double dot = mlDot(of, fp);
		double fp2 = mlSqLength(fp);
		
		return fp2 / (sqrt(dot * dot - fp2 * c) - dot); 
	}
	
private:
	MLVec2D o, of, rinv;
	double c;
};

#endif // MLGRADIENTGENERATOR_H
