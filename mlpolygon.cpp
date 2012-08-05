#include "mlpolygon.h"

MLPolygon::MLPolygon(const QVector<QPointF> &points) :
	QVector<MLVec2D>(points.size())
{
	memcpy(data(), points.constData(), points.size() * sizeof(double) * 2);
}

QPolygonF MLPolygon::toQPolygonF() const
{
	QPolygonF polygon(size());
	memcpy(polygon.data(), constData(), size() * sizeof(double) * 2);
	return polygon;
}
