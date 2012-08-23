#ifndef MLDIVISION_H
#define MLDIVISION_H

#include "mlglobal.h"
#include <QPoint>
#include <cmath>

// 余りが必ず正になる整数除算
class MALACHITESHARED_EXPORT MLIntDivision
{
public:
	MLIntDivision(int numerator, int denominator) {
		result = div(numerator, denominator);
		if (result.rem < 0) {
			result.rem += denominator;
			result.quot--;
		}
	}
	
	int quot() const { return result.quot; }
	int rem() const { return result.rem; }
	
private:
	div_t result;
};

class MALACHITESHARED_EXPORT MLDivision
{
public:
	MLDivision(double numerator, double denominator)
	{
		_quot = floor(numerator / denominator);
		_rem = fmod(numerator, denominator);
		
		if (_rem < 0)
		{
			_rem += denominator;
			_quot -= 1;
		}
	}
	
	double quot() const { return _quot; }
	double rem() const { return _rem; }
	
private:
	double _quot, _rem;
};

inline void mlDividePoint(const QPoint &point, int divisor, QPoint *quot, QPoint *rem)
{
	MLIntDivision divisionX(point.x(), divisor);
	MLIntDivision divisionY(point.y(), divisor);
	
	quot->rx() = divisionX.quot();
	quot->ry() = divisionY.quot();
	rem->rx() = divisionX.rem();
	rem->ry() = divisionY.rem();
}

inline void mlDividePoint(const QPoint &point, int divisor, QPoint *quot)
{
	MLIntDivision divisionX(point.x(), divisor);
	MLIntDivision divisionY(point.y(), divisor);
	
	quot->rx() = divisionX.quot();
	quot->ry() = divisionY.quot();
}

inline double mlAlign(double value, double unit)
{
	return round(value / unit) * unit;
}

#endif // MLDIVISION_H
