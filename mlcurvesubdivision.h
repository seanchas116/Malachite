#ifndef MLCURVESUBDIVISION_H
#define MLCURVESUBDIVISION_H

#include "mlpolygon.h"
#include "mlcurves.h"
#include "mlvector.h"
#include "private/agg_array.h"

namespace Malachite
{

class CurveSubdivision
{
public:
	CurveSubdivision(const Curve4 &curve);
	CurveSubdivision(const Vec2D &start, const Vec2D &control1, const Vec2D &control2, const Vec2D &end);
	
	Polygon polygon() const { return _polygon; }
	
private:
	Polygon _polygon;
};

}

#endif // MLCURVESUBDIVISION_H
