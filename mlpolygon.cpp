#include "mlcurvesubdivision.h"

#include "mlpolygon.h"

namespace Malachite
{

Polygon::Polygon(const QVector<QPointF> &points) :
	QVector<Vec2D>(points.size())
{
	memcpy(data(), points.constData(), points.size() * sizeof(double) * 2);
}

Polygon &Polygon::operator*=(const QTransform &transform)
{
	Pointer<Vec2D> p(data(), size() * sizeof(Vec2D));
	
	for (Polygon::iterator i = begin(); i != end(); ++i)
	{
		Vec2D result;
		transform.map(i->x, i->y, &result.x, &result.y);
		*i = result;
	}
	
	return *this;
}

QPolygonF Polygon::toQPolygonF() const
{
	QPolygonF polygon(size());
	memcpy(polygon.data(), constData(), size() * sizeof(double) * 2);
	return polygon;
}

Polygon Polygon::fromRect(const QRectF &rect)
{
	Polygon polygon(4);
	polygon[0] = Vec2D(rect.x(), rect.y());
	polygon[1] = Vec2D(rect.x() + rect.width(), rect.y());
	polygon[2] = Vec2D(rect.x() + rect.width(), rect.y() + rect.height());
	polygon[3] = Vec2D(rect.x(), rect.y() + rect.height());
	return polygon;
}


MultiPolygon MultiPolygon::fromQPainterPath(const QPainterPath &path)
{
	int count = path.elementCount();
	
	Polygon polygon;
	MultiPolygon polygons;
	
	int i = 0;
	while (i < count)
	{
		const QPainterPath::Element elem = path.elementAt(i);
		
		switch (elem.type)
		{
			case QPainterPath::CurveToElement:
			{
				QPainterPath::Element e1, e2, e3, e4;
				e1 = path.elementAt(i - 1);
				e2 = elem;
				e3 = path.elementAt(i + 1);
				e4 = path.elementAt(i + 2);
				
				polygon << CurveSubdivision(Vec2D(e1.x, e1.y), Vec2D(e2.x, e2.y), Vec2D(e3.x, e3.y), Vec2D(e4.x, e4.y)).polygon().mid(1);
				i += 3;
				break;
			}
			case QPainterPath::MoveToElement:
			{
				if (polygon.size() > 2)
					polygons << polygon;
				polygon = Polygon();
			}	// fall through
			case QPainterPath::LineToElement:
			{
				polygon << Vec2D(elem.x, elem.y);
			}	// fall through
			default:
			{
				++i;
				break;
			}
		}
	}
	
	if (polygon.size() > 2)
		polygons << polygon;
	
	return polygons;
}

MultiPolygon &MultiPolygon::operator*=(const QTransform &transform)
{
	for (MultiPolygon::iterator i = begin(); i != end(); ++i)
		*i *= transform;
	
	return *this;
}

}

