#ifndef MLFIXEDPOLYGON_H
#define MLFIXEDPOLYGON_H

#include <QVector>
#include "mlpolygon.h"

struct MLFixedPoint
{
	enum
	{
		SubpixelWidth = 8,
		SubpixelPrecision = 1 << SubpixelWidth
	};
	
	int64_t intToFixed(int64_t x)
	{
		uint64_t data = mlTransferCast<uint64_t>(x);
		uint64_t sign = data & (uint64_t(1) << 63);
		data <<= SubpixelWidth;
		data |= sign;
		return mlTransferCast<int64_t>(data);
	}
	
	MLFixedPoint() {}
	MLFixedPoint(int x, int y) : x(intToFixed(x)), y(intToFixed(y)) {}
	MLFixedPoint(const QPoint &p) : MLFixedPoint(p.x(), p.y()) {}
	MLFixedPoint(double x, double y) : MLFixedPoint(MLVec2D(x, y)) {}
	
	MLFixedPoint(const MLVec2D &p)
	{
		MLVec2D v = p * MLVec2D(SubpixelPrecision);
		MLVec4I32 iv = mlRound(v);
		x = iv.x;
		y = iv.y;
	}
	
	static MLFixedPoint fromRawData(int64_t x, int64_t y)
	{
		MLFixedPoint result;
		result.x = x;
		result.y = y;
		return result;
	}
	
	MLFixedPoint &operator+=(const MLFixedPoint &p)
	{
		x += p.x;
		y += p.y;
		return *this;
	}
	
	MLVec2D toMLVec2D() const
	{
		MLVec2D result(x, y);
		result *= (1.0 / SubpixelPrecision);
		return result;
	}
	
	int64_t x, y;	// For compatibility with ClipperLib
};

class MLFixedPolygon : public QVector<MLFixedPoint>
{
public:
	MLFixedPolygon() : QVector<MLFixedPoint>() {}
	MLFixedPolygon(int size) : QVector<MLFixedPoint>(size) {}
	MLFixedPolygon(const MLPolygon &polygon);
	
	void translate(const MLFixedPoint &delta);
	
	static MLFixedPolygon fromRect(const QRectF &rect) { return MLPolygon::fromRect(rect); }
	static MLFixedPolygon fromRect(const QRect &rect);
};

class MLFixedMultiPolygon : public QVector<MLFixedPolygon>
{
public:
	MLFixedMultiPolygon() : QVector<MLFixedPolygon>() {}
	MLFixedMultiPolygon(int size) : QVector<MLFixedPolygon>(size) {}
	MLFixedMultiPolygon(const MLFixedPolygon &polygon) : MLFixedMultiPolygon(1) { operator[](0) = polygon; }
	
	void translate(const MLFixedPoint &delta)
	{
		for (auto i = begin(); i != end(); ++i)
			i->translate(delta);
	}
	
	QRectF boundingRect() const;
	
	static MLFixedMultiPolygon fromMLPolygons(const MLMultiPolygon &polygons);
	static MLFixedMultiPolygon fromQPainterPath(const QPainterPath &path) { return fromMLPolygons(MLMultiPolygon::fromQPainterPath(path)); }
};

MLFixedMultiPolygon operator|(const MLFixedMultiPolygon &polygons1, const MLFixedMultiPolygon &polygons2);
MLFixedMultiPolygon operator&(const MLFixedMultiPolygon &polygons1, const MLFixedMultiPolygon &polygons2);
MLFixedMultiPolygon operator^(const MLFixedMultiPolygon &polygons1, const MLFixedMultiPolygon &polygons2);


#endif // MLFIXEDPOLYGON_H
