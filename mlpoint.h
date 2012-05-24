#ifndef MLPOINT_H
#define MLPOINT_H

#include "mlglobal.h"
#include <QPointF>

class MALACHITESHARED_EXPORT MLPoint
{
public:
	MLPoint() : _x(0), _y(0) {}
	MLPoint(float x, float y) : _x(x), _y(y) {}
	MLPoint(const QPointF &qp) : _x(qp.x()), _y(qp.y()) {}
	
	float x() const { return _x; }
	float y() const { return _y; }
	void setX(float x) { _x = x; }
	void setY(float y) { _y = y; }
	
	MLPoint &operator=(const QPointF &qp)
	{
		_x = qp.x();
		_y = qp.y();
		return *this;
	}
	
	MLPoint &operator+=(const MLPoint other)
	{
		_x += other._x;
		_y += other._y;
		return *this;
	}
	
	MLPoint &operator-=(const MLPoint other)
	{
		_x -= other._x;
		_y -= other._y;
		return *this;
	}
	
	MLPoint &operator+=(float value)
	{
		_x += value;
		_y += value;
		return *this;
	}
	
	MLPoint &operator-=(float value)
	{
		_x -= value;
		_y -= value;
		return *this;
	}
	
	MLPoint &operator*=(float value)
	{
		_x *= value;
		_y *= value;
		return *this;
	}
	
	MLPoint &operator/=(float value)
	{
		_x /= value;
		_y /= value;
		return *this;
	}
	
private:
	float _x, _y;
};

inline MLPoint operator+(const MLPoint &p1, const MLPoint &p2)
{
	MLPoint p = p1;
	p += p2;
	return p;
}

inline MLPoint operator-(const MLPoint &p1, const MLPoint &p2)
{
	MLPoint p = p1;
	p -= p2;
	return p;
}

inline MLPoint operator+(const MLPoint &p1, float value)
{
	MLPoint p = p1;
	p += value;
	return p;
}

inline MLPoint operator-(const MLPoint &p1, float value)
{
	MLPoint p = p1;
	p -= value;
	return p;
}

inline MLPoint operator*(const MLPoint &p1, float value)
{
	MLPoint p = p1;
	p *= value;
	return p;
}

inline MLPoint operator/(const MLPoint &p1, float value)
{
	MLPoint p = p1;
	p /= value;
	return p;
}

#endif // MLPOINT_H
