#ifndef MLPOLYGON_H
#define MLPOLYGON_H

#include "mlvector.h"
#include <QVector>
#include <QPolygonF>

class MLPolygon : public QVector<MLVec2D>
{
public:
	MLPolygon() : QVector<MLVec2D>() {}
	MLPolygon(int size) : QVector<MLVec2D>(size) {}
	MLPolygon(const QVector<QPointF> &points);
	
	static MLPolygon fromRect(const QRectF &rect);
	
	MLPolygon &operator*=(const QTransform &transform);
	
	QPolygonF toQPolygonF() const;
};

class MLMultiPolygon : public QVector<MLPolygon>
{
public:
	MLMultiPolygon() : QVector<MLPolygon>() {}
	MLMultiPolygon(int size) : QVector<MLPolygon>(size) {}
	MLMultiPolygon(const MLPolygon &polygon) : QVector<MLPolygon>(1) { operator[](0) = polygon; }
	
	static MLMultiPolygon fromQPainterPath(const QPainterPath &path);
	
	MLMultiPolygon &operator*=(const QTransform &transform);
};

inline MLMultiPolygon operator*(const MLMultiPolygon &polygons, const QTransform &transform)
{
	MLMultiPolygon result = polygons;
	result *= transform;
	return result;
}

#endif // MLPOLYGON_H
