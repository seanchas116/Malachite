#ifndef MLCOLORGRADIENT_H
#define MLCOLORGRADIENT_H

#include "mlcolor.h"
#include "mlgenericgradient.h"


class MLColorGradientCache;

class MALACHITESHARED_EXPORT MLColorGradient
{
	friend class MLColorGradientCache;
	
public:
	
	MLColorGradient() : _type(MLGlobal::GradientTypeNull) {}
	
	MLGlobal::GradientType type() { return _type; }
	
	void addStop(float x, const MLArgb &y) { _gradient.addStop(x, y.v); }
	void addStop(float x, const MLColor &color) { addStop(x, color.toArgb()); }
	
	void clear() { _gradient.clear(); }
	
	MLArgb at(float x) const
	{
		MLArgb r;
		r.v = _gradient.at(x);
		return r;
	}
	
protected:
	
	void setType(MLGlobal::GradientType type) { _type = type; }
	
private:
	
	MLGlobal::GradientType _type;
	MLGenericLineGradient<float, MLSimdF4> _gradient;
};

class MLColorGradientCache
{
public:
	
	MLColorGradientCache() : _cache(256) {}
	
	void load(const MLColorGradient &gradient)
	{
		_cache.load(gradient._gradient);
	}
	
	MLArgb at(float x) const
	{
		MLArgb r;
		r.v = _cache.at(x);
		return r;
	}
	
private:
	
	MLGenericGradientCache<float, MLSimdF4> _cache;
};

class MALACHITESHARED_EXPORT MLLinearColorGradient : public MLColorGradient
{
public:
	
	MLLinearColorGradient(const QPointF &start, const QPointF &end) :
		_start(start),
		_end(end)
	{
		setType(MLGlobal::GradientTypeLinear);
	}
	
	QPointF start() const { return _start; }
	QPointF end() const { return _end; }
	
private:
	QPointF _start, _end;
};

class MALACHITESHARED_EXPORT MLRadialColorGradient : public MLColorGradient
{
public:
	
	MLRadialColorGradient(const QPointF &center, double radius, const QPointF &focal) :
		_center(center),
		_focal(focal),
		_radius(radius)
	{
		setType(MLGlobal::GradientTypeRadial);
	}
	
	MLRadialColorGradient(const QPointF &center, double radius) :
		_center(center),
		_focal(center),
		_radius(radius)
	{
		setType(MLGlobal::GradientTypeRadial);
	}
	
	QPointF center() const { return _center; }
	QPointF focal() const { return _focal; }
	double radius() const { return _radius; }
	
private:
	QPointF _center, _focal;
	double _radius;
};

Q_DECLARE_METATYPE(MLColorGradient)
Q_DECLARE_METATYPE(MLColorGradientCache)


#endif // MLCOLORGRADIENT_H
