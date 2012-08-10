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

struct MLLinearGradientInfo
{
	MLLinearGradientInfo() {}
	
	MLLinearGradientInfo(const MLVec2D &start, const MLVec2D &end) :
		start(start), end(end) {}
	
	bool transformable(const QTransform &transform) const
	{
		if (transform.isIdentity())
			return true;
		
		if (transform.isAffine() && transform.m12() == 0 && transform.m21() == 0 && transform.m11() == transform.m22())
			return true;
		
		return false;
	}
	
	void transform(const QTransform &transform)
	{
		start *= transform;
		end *= transform;
	}
	
	MLVec2D start, end;
};

struct MLRadialGradientInfo
{
	MLRadialGradientInfo() {}
	
	MLRadialGradientInfo(const MLVec2D &center, double radius, const MLVec2D &focal) :
		center(center), focal(focal), radius(radius) {}
	
	MLRadialGradientInfo(const MLVec2D &center, const MLVec2D &radius, const MLVec2D &focal) :
		center(center), focal(focal), radius(radius) {}
	
	MLRadialGradientInfo(const MLVec2D &center, double radius) :
		center(center), focal(center), radius(radius) {}
	
	MLRadialGradientInfo(const MLVec2D &center, const MLVec2D &radius) :
		center(center), focal(center), radius(radius) {}
	
	bool transformable(const QTransform &transform) const
	{
		if (transform.isIdentity())
			return true;
		
		if (transform.isRotating() && radius.x == radius.y)
			return true;
		
		if (transform.isAffine() && transform.m12() == 0 && transform.m21() == 0)
			return true;
		
		return false;
	}
	
	void transform(const QTransform &transform)
	{
		center *= transform;
		focal *= transform;
		radius *= MLVec2D(transform.m11(), transform.m22());
	}
	
	MLVec2D center, focal, radius;
};

Q_DECLARE_METATYPE(MLLinearGradientInfo)
Q_DECLARE_METATYPE(MLRadialGradientInfo)



#endif // MLCOLORGRADIENT_H
