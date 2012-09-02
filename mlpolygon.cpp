#include "mlcurvesubdivision.h"

#include "mlpolygon.h"

MLPolygon::MLPolygon(const QVector<QPointF> &points) :
	QVector<MLVec2D>(points.size())
{
	memcpy(data(), points.constData(), points.size() * sizeof(double) * 2);
}

MLPolygon &MLPolygon::operator*=(const QTransform &transform)
{
	MLPointer<MLVec2D> p(data(), size() * sizeof(MLVec2D));
	
	for (MLPolygon::iterator i = begin(); i != end(); ++i)
	{
		MLVec2D result;
		transform.map(i->x, i->y, &result.x, &result.y);
		*i = result;
	}
	
	return *this;
}

QPolygonF MLPolygon::toQPolygonF() const
{
	QPolygonF polygon(size());
	memcpy(polygon.data(), constData(), size() * sizeof(double) * 2);
	return polygon;
}

MLPolygon MLPolygon::fromRect(const QRectF &rect)
{
	MLPolygon polygon(4);
	polygon[0] = MLVec2D(rect.x(), rect.y());
	polygon[1] = MLVec2D(rect.x() + rect.width(), rect.y());
	polygon[2] = MLVec2D(rect.x() + rect.width(), rect.y() + rect.height());
	polygon[3] = MLVec2D(rect.x(), rect.y() + rect.height());
	return polygon;
}


MLMultiPolygon MLMultiPolygon::fromQPainterPath(const QPainterPath &path)
{
	int count = path.elementCount();
	
	MLPolygon polygon;
	MLMultiPolygon polygons;
	
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
				
				polygon << MLCurveSubdivision(MLVec2D(e1.x, e1.y), MLVec2D(e2.x, e2.y), MLVec2D(e3.x, e3.y), MLVec2D(e4.x, e4.y)).polygon().mid(1);
				i += 3;
				break;
			}
			case QPainterPath::MoveToElement:
			{
				if (polygon.size() > 2)
					polygons << polygon;
				polygon = MLPolygon();
			}	// fall through
			case QPainterPath::LineToElement:
			{
				polygon << MLVec2D(elem.x, elem.y);
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

MLMultiPolygon &MLMultiPolygon::operator*=(const QTransform &transform)
{
	for (MLMultiPolygon::iterator i = begin(); i != end(); ++i)
		*i *= transform;
	
	return *this;
}



