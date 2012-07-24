#ifndef MLINTDIVISION_H
#define MLINTDIVISION_H

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
	
	static void dividePoint(const QPoint &point, int divisor, QPoint *quot, QPoint *rem);
	static void dividePoint(const QPoint &point, int divisor, QPoint *quot);
	
private:
	div_t result;
};

inline void MLIntDivision::dividePoint(const QPoint &point, int divisor, QPoint *quot, QPoint *rem)
{
	MLIntDivision divisionX(point.x(), divisor);
	MLIntDivision divisionY(point.y(), divisor);
	
	quot->rx() = divisionX.quot();
	quot->ry() = divisionY.quot();
	rem->rx() = divisionX.rem();
	rem->ry() = divisionY.rem();
}

inline void MLIntDivision::dividePoint(const QPoint &point, int divisor, QPoint *quot)
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

#endif // MLINTDIVISION_H
