#include <QtGui>

#include "agg_curves.h"
#include "mlsegmentpath.h"

MLSegmentPath::MLSegmentPath(const QPainterPath &path)
{
	for (int i = 0; i < path.elementCount(); ++i)
	{
		const QPainterPath::Element e = path.elementAt(i);
		
		switch (e.type)
		{
		case QPainterPath::CurveToElement:
		{
			const QPainterPath::Element e1 = path.elementAt(i-1);
			const QPainterPath::Element e2 = path.elementAt(i);
			const QPainterPath::Element e3 = path.elementAt(i+1);
			const QPainterPath::Element e4 = path.elementAt(i+2);
			
			agg::curve4_div curve4(e1.x, e1.y, e2.x, e2.y, e3.x, e3.y, e4.x, e4.y);
			
			forever
			{
				double x, y;
				unsigned type = curve4.vertex(&x, &y);
				
				if (type == agg::path_cmd_line_to)
					lineTo(x, y);
			}
		}
			break;
		case QPainterPath::MoveToElement:
			moveTo(e.x, e.y);
			break;
		case QPainterPath::LineToElement:
			lineTo(e.x, e.y);
			break;
		default:
			break;
		}
	}
}

MLSegmentPath::MLSegmentPath(const QPolygonF &polygon)
{
	_polygons << polygon;
}

MLSegmentPath::MLSegmentPath(const MLPolygon &polygon)
{
	_polygons << polygon;
}

void MLSegmentPath::moveTo(const QPointF &point)
{
	QPolygonF polygon;
	polygon << point;
	_polygons << polygon;
}

void MLSegmentPath::lineTo(const QPointF &point)
{
	_polygons.last() << point;
}


