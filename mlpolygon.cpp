#include "mlpolygon.h"

MLPolygon::MLPolygon(const QPolygonF &polygon) :
	QVector<MLPoint>(polygon.size())
{
	QVector<MLPoint>::iterator i = begin();
	
	foreach (const QPointF &p, polygon)
	{
		i->setX(p.x());
		i->setY(p.y());
		++i;
	}
}
