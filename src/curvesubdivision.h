#ifndef MLCURVESUBDIVISION_H
#define MLCURVESUBDIVISION_H

//ExportName: CurveSubdivision

#include "polygon.h"
#include "curves.h"

namespace Malachite
{

class MALACHITESHARED_EXPORT CurveSubdivision
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
