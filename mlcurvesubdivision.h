#ifndef MLCURVESUBDIVISION_H
#define MLCURVESUBDIVISION_H

#include "mlpolygon.h"
#include "mlcurves.h"
#include "mlvector.h"
#include "private/agg_array.h"

class MLCurveSubdivision
{
public:
	MLCurveSubdivision(const MLCurve4 &curve);
	MLCurveSubdivision(const MLVec2D &start, const MLVec2D &control1, const MLVec2D &control2, const MLVec2D &end);
	
	MLPolygon polygon() const { return _polygon; }
	
private:
	MLPolygon _polygon;
};

#endif // MLCURVESUBDIVISION_H
