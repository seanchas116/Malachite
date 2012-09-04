#include "private/clipper.hpp"
#include "mlfixedpolygon.h"

MLFixedPolygon::MLFixedPolygon(const MLPolygon &polygon) :
    MLFixedPolygon(polygon.size())
{
	int size = polygon.size();
	
	MLFixedPoint *dst = data();
	const MLVec2D *src = polygon.constData();
	
	for (int i = 0; i < size; ++i)
		*dst++ = MLFixedPoint(*src++);
}

void MLFixedPolygon::translate(const MLFixedPoint &delta)
{
	MLFixedPoint *p = data();
	
	for (int i = 0; i < size(); ++i)
		*p++ += delta;
}

MLFixedPolygon MLFixedPolygon::fromRect(const QRect &rect)
{
	MLFixedPolygon result(4);
	result[0] = MLFixedPoint(rect.left(), rect.top());
	result[1] = MLFixedPoint(rect.left() + rect.width(), rect.top());
	result[2] = MLFixedPoint(rect.left() + rect.width(), rect.top() + rect.height());
	result[3] = MLFixedPoint(rect.left(), rect.top() + rect.height());
	return result;
}

MLFixedMultiPolygon MLFixedMultiPolygon::fromMLPolygons(const MLMultiPolygon &polygons)
{
	MLFixedMultiPolygon result;
	foreach (const MLPolygon &polygon, polygons)
	{
		result << polygon;
	}
	return result;
}

QRectF MLFixedMultiPolygon::boundingRect() const
{
	MLFixedMultiPolygon polys = *this;
	
	for (int i = 0; i < polys.size(); ++i)
	{
		if (polys.at(i).size() == 0)
			polys.remove(i);
	}
	
	if (polys.size() == 0)
		return QRectF();
	
	MLFixedPoint min, max;
	min = max = polys.at(0).at(0);
	
	foreach (const MLFixedPolygon &poly, polys)
	{
		foreach (const MLFixedPoint &p, poly)
		{
			if (p.x < min.x)
				min.x = p.x;
			if (p.y < min.y)
				min.y = p.y;
			if (p.x > max.x)
				max.x = p.x;
			if (p.y > max.y)
				max.y = p.y;
		}
	}
	
	MLVec2D xy, wh;
	xy = min.toMLVec2D();
	wh = max.toMLVec2D() - xy;
	
	return QRectF(xy.x, xy.y, wh.x, wh.y);
}

using namespace ClipperLib;

MLFixedMultiPolygon operator|(const MLFixedMultiPolygon &polygons1, const MLFixedMultiPolygon &polygons2)
{
	MLFixedMultiPolygon result;
	
	Clipper clipper;
	clipper.AddPolygons(mlTransferCast<const Polygons>(polygons1), ptSubject);
	clipper.AddPolygons(mlTransferCast<const Polygons>(polygons2), ptClip);
	clipper.Execute(ctUnion, mlTransferCast<Polygons>(result));
	
	return result;
}

MLFixedMultiPolygon operator&(const MLFixedMultiPolygon &polygons1, const MLFixedMultiPolygon &polygons2)
{
	MLFixedMultiPolygon result;
	
	Clipper clipper;
	clipper.AddPolygons(mlTransferCast<const Polygons>(polygons1), ptSubject);
	clipper.AddPolygons(mlTransferCast<const Polygons>(polygons2), ptClip);
	clipper.Execute(ctIntersection, mlTransferCast<Polygons>(result));
	
	return result;
}

MLFixedMultiPolygon operator^(const MLFixedMultiPolygon &polygons1, const MLFixedMultiPolygon &polygons2)
{
	MLFixedMultiPolygon result;
	
	Clipper clipper;
	clipper.AddPolygons(mlTransferCast<const Polygons>(polygons1), ptSubject);
	clipper.AddPolygons(mlTransferCast<const Polygons>(polygons2), ptClip);
	clipper.Execute(ctXor, mlTransferCast<Polygons>(result));
	
	return result;
}

MLFixedMultiPolygon operator-(const MLFixedMultiPolygon &polygons1, const MLFixedMultiPolygon &polygons2)
{
	if (polygons2.size() == 0)
		return polygons1;
	
	MLFixedMultiPolygon result;
	
	Clipper clipper;
	clipper.AddPolygons(mlTransferCast<const Polygons>(polygons1), ptSubject);
	clipper.AddPolygons(mlTransferCast<const Polygons>(polygons2), ptClip);
	clipper.Execute(ctDifference, mlTransferCast<Polygons>(result));
	
	return result;
}
