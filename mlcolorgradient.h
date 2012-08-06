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
	
	virtual MLArgb at(float x) const = 0;
	virtual MLColorGradient *clone() const { return 0; }
};

class MALACHITESHARED_EXPORT MLArgbGradient : public MLColorGradient
{
public:
	
	MLArgbGradient() : MLColorGradient() {}
	
	void addStop(float x, const MLArgb &y) { _stops.insert(x, y); }
	void addStop(float x, const MLColor &color) { addStop(x, color.toArgb()); }
	void clear() { _stops.clear(); }
	
	MLArgb at(float x) const
	{
		int count = _stops.size();
		if (count == 0)
			return MLArgb();
		if (count == 1)
			return _stops.value(0);
		
		QMapIterator<float, MLArgb> i(_stops);
		
		if (x <= i.peekNext().key()) return i.peekNext().value();
		i.next();
		
		for (; i.hasNext(); i.next())
		{
			float x1 = i.peekNext().key();
			MLArgb y1 = i.peekNext().value();
			if (x < x1)
			{
				float x0 = i.peekPrevious().key();
				MLArgb y0 = i.peekPrevious().value();
				
				MLArgb r;
				r.v = y0.v + (x - x0) / (x1 - x0) * (y1.v - y0.v);
				return r;
			}
			if (x == x1)
				return y1;
		}
		
		return _stops.values().at(0);
	}
	
	MLColorGradient *clone() const { return new MLArgbGradient(*this); }
	
private:
	
	QMap<float, MLArgb> _stops;
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
