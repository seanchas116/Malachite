#ifndef MLCOLORGRADIENT_H
#define MLCOLORGRADIENT_H

#include "mlcolor.h"
#include "mlgenericgradient.h"
#include <QDebug>

class MALACHITESHARED_EXPORT MLColorGradient
{
public:
	MLColorGradient() {}
	virtual ~MLColorGradient() {}
	
	virtual MLArgb at(float x) const = 0;
	
protected:
};

class MLColorGradientCache;

class MALACHITESHARED_EXPORT MLArgbGradient : public MLColorGradient
{
	friend class MLColorGradientCache;
	
public:
	
	MLArgbGradient() : MLColorGradient() {}
	
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
	
private:
	
	MLGenericLineGradient<float, MLSimdF4> _gradient;
};

class MLColorGradientCache : public MLColorGradient
{
public:
	
	MLColorGradientCache() : MLColorGradient(), _cache(256) {}
	
	void load(const MLColorGradient &gradient)
	{
		for (int i = 0; i <= _cache.size(); ++i)
		{
			_cache[i] = gradient.at((float)i / (float)_cache.size()).v;
		}
	}
	
	MLArgb at(float x) const
	{
		MLArgb color;
		color.v = _cache.at(x);
		return color;
	}
	
protected:
	
private:
	
	MLGenericGradientCache<float, MLSimdF4> _cache;
};

class MLLinearGradientInfo
{
public:
	
	MLLinearGradientInfo() {}
	
	MLLinearGradientInfo(const QPointF &start, const QPointF &end) :
		_start(start), _end(end) {}
	
	QPointF start() const { return _start; }
	QPointF end() const { return _end; }
	void setStart(const QPointF &p) { _start = p; }
	void setEnd(const QPointF &p) { _end = p; }
	
private:
	
	QPointF _start, _end;
};

class MLRadialGradientInfo
{
public:
	
	MLRadialGradientInfo() {}
	
	MLRadialGradientInfo(const QPointF &center, double radius, const QPointF &focal) :
		_center(center), _focal(focal), _radius(radius) {}
	
	MLRadialGradientInfo(const QPointF &center, double radius) :
		_center(center), _focal(center), _radius(radius) {}
	
	QPointF center() const { return _center; }
	QPointF focal() const { return _focal; }
	double radius() const { return _radius; }
	void setCenter(const QPointF &p) { _center = p; }
	void setFocal(const QPointF &p) { _focal = p; }
	void setRadius(double r) { _radius = r; }
	
private:
	
	QPointF _center, _focal;
	double _radius;
};

Q_DECLARE_METATYPE(MLLinearGradientInfo)
Q_DECLARE_METATYPE(MLRadialGradientInfo)



#endif // MLCOLORGRADIENT_H
