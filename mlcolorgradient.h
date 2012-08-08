#ifndef MLCOLORGRADIENT_H
#define MLCOLORGRADIENT_H

#include "mlcolor.h"
#include "mlvector.h"
#include <QMap>

class MALACHITESHARED_EXPORT MLColorGradient
{
public:
	MLColorGradient() {}
	virtual ~MLColorGradient() {}
	
	virtual MLVec4F at(float x) const = 0;
	virtual MLColorGradient *clone() const { return 0; }
};

class MALACHITESHARED_EXPORT MLColorGradientCache : public MLColorGradient
{
public:
	
	MLColorGradientCache(MLColorGradient *gradient, int sampleCount);
	
	MLVec4F at(float x) const
	{
		return _cache.at(roundf(x * _sampleCount));
	}
	
	int sampleCount() const { return _sampleCount; }
	
private:
	
	int _sampleCount;
	QVector<MLVec4F> _cache;
};

class MALACHITESHARED_EXPORT MLArgbGradient : public MLColorGradient
{
public:
	
	MLArgbGradient() : MLColorGradient() {}
	
	void addStop(float x, const MLVec4F &y) { _stops.insert(x, y); }
	void addStop(float x, const MLColor &color) { addStop(x, color.toArgb()); }
	void clear() { _stops.clear(); }
	
	MLVec4F at(float x) const;
	
	MLColorGradient *clone() const { return new MLArgbGradient(*this); }
	
private:
	
	QMap<float, MLVec4F> _stops;
};

class MLLinearGradientInfo
{
public:
	
	MLLinearGradientInfo() {}
	
	MLLinearGradientInfo(const MLVec2D &start, const MLVec2D &end) :
		_start(start), _end(end) {}
	
	MLVec2D start() const { return _start; }
	MLVec2D end() const { return _end; }
	void setStart(const MLVec2D &p) { _start = p; }
	void setEnd(const MLVec2D &p) { _end = p; }
	
private:
	
	MLVec2D _start, _end;
};

class MLRadialGradientInfo
{
public:
	
	MLRadialGradientInfo() {}
	
	MLRadialGradientInfo(const MLVec2D &center, double radius, const MLVec2D &focal) :
		_center(center), _focal(focal), _radius(radius) {}
	
	MLRadialGradientInfo(const MLVec2D &center, double radius) :
		_center(center), _focal(center), _radius(radius) {}
	
	MLVec2D center() const { return _center; }
	MLVec2D focal() const { return _focal; }
	double radius() const { return _radius; }
	void setCenter(const MLVec2D &p) { _center = p; }
	void setFocal(const MLVec2D &p) { _focal = p; }
	void setRadius(double r) { _radius = r; }
	
private:
	
	MLVec2D _center, _focal;
	double _radius;
};

Q_DECLARE_METATYPE(MLLinearGradientInfo)
Q_DECLARE_METATYPE(MLRadialGradientInfo)



#endif // MLCOLORGRADIENT_H
