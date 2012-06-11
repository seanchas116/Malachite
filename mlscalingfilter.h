#ifndef MLSCALINGFILTER_H
#define MLSCALINGFILTER_H

#include "mlcolor.h"

inline double mlSinc(double x)
{
	if (x == 0) return 1;
	return sin(M_PI * x) / (M_PI * x);
}

class MLScalingFilterLanczos2Hypot
{
public:
	
	static double weight(const QPointF &delta)
	{
		double d = hypot(delta.x(), delta.y());
		if (d == 0) return 1;
		return mlSinc(d) * mlSinc(d * 0.5);
	}
	
	static int range() { return 2; }
};


#endif // MLSCALINGFILTER_H
