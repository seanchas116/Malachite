#include <QPolygonF>
#include "mlpolygon.h"

#include "mlmisc.h"


double mlCalculatePolygonSpace(const QPolygonF &polygon)
{
	if (polygon.size() < 3)
		return 0;
	
	QVectorIterator<QPointF> i(polygon);
	double space = 0;
	
	for (i.next(); i.hasNext(); i.next())
	{
		QPointF next = i.peekNext();
		QPointF prev = i.peekPrevious();
		
		space += (next.y() + prev.y()) * (next.x() - prev.x());
	}
	
	return fabs(space * 0.5);
}

float mlCalculatePolygonSpace(const MLPolygon &polygon)
{
	if (polygon.size() < 3)
		return 0;
	
	QVectorIterator<MLPoint> i(polygon);
	float space = 0;
	
	for (i.next(); i.hasNext(); i.next())
	{
		MLPoint next = i.peekNext();
		MLPoint prev = i.peekPrevious();
		
		space += (next.y() + prev.y()) * (next.x() - prev.x());
	}
	
	return fabsf(space * 0.5f);
}
