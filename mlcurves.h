#ifndef MLCURVES_H
#define MLCURVES_H

#include "mlpolygon.h"
#include "mlglobal.h"
#include "mlvector.h"

class MALACHITESHARED_EXPORT MLCurve4
{
public:
	MLCurve4() {}
	MLCurve4(const MLVec2D &start, const MLVec2D &control1, const MLVec2D &control2, const MLVec2D &end)
	{
		this->start = start;
		this->control1 = control1;
		this->control2 = control2;
		this->end = end;
	}
	
	static MLCurve4 fromCatmullRom(const MLVec2D &previous, const MLVec2D &start, const MLVec2D &end, const MLVec2D &next)
	{
		return MLCurve4(start, start + (end - previous) / 6.0, end - (next - start) / 6.0, end);
	}
	
	static MLCurve4 fromCatmullRomStart(const MLVec2D &start, const MLVec2D &end, const MLVec2D &next)
	{
		return MLCurve4(start, start + (end - start) / 3.0, end - (next - start) / 6.0, end);
	}
	
	static MLCurve4 fromCatmullRomEnd(const MLVec2D &previous, const MLVec2D &start, const MLVec2D &end)
	{
		return MLCurve4(start, start + (end - previous) / 6.0, (end - start) / 3.0, end);
	}
	
	static MLCurve4 fromCatmullRom(const MLPolygon &polygon, int indexStart);
	
	MLVec2D start, control1, control2, end;
};

#endif // MLCURVES_H
