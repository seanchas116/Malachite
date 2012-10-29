#ifndef MLCURVES_H
#define MLCURVES_H

//ExportName: Curves

#include "polygon.h"
#include "global.h"
#include "vector.h"

namespace Malachite
{

class MALACHITESHARED_EXPORT Curve4
{
public:
	Curve4() {}
	Curve4(const Vec2D &start, const Vec2D &control1, const Vec2D &control2, const Vec2D &end)
	{
		this->start = start;
		this->control1 = control1;
		this->control2 = control2;
		this->end = end;
	}
	
	static Curve4 fromCatmullRom(const Vec2D &previous, const Vec2D &start, const Vec2D &end, const Vec2D &next)
	{
		return Curve4(start, start + (end - previous) / 6.0, end - (next - start) / 6.0, end);
	}
	
	static Curve4 fromCatmullRomStart(const Vec2D &start, const Vec2D &end, const Vec2D &next)
	{
		return Curve4(start, start + (end - start) / 3.0, end - (next - start) / 6.0, end);
	}
	
	static Curve4 fromCatmullRomEnd(const Vec2D &previous, const Vec2D &start, const Vec2D &end)
	{
		return Curve4(start, start + (end - previous) / 6.0, (end - start) / 3.0, end);
	}
	
	static Curve4 fromCatmullRom(const Polygon &polygon, int indexStart);
	
	Vec2D start, control1, control2, end;
};

}

#endif // MLCURVES_H
