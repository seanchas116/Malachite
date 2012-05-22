#ifndef MLSEGMENTPATH_H
#define MLSEGMENTPATH_H

#include <QPainterPath>
#include "mlglobal.h"

class MALACHITESHARED_EXPORT MLSegmentPath
{
public:
	MLSegmentPath() {}
	MLSegmentPath(const QPainterPath &path);
	MLSegmentPath(const QPolygonF &polygon);
	
	void moveTo(const QPointF &point);
	void moveTo(double x, double y) { moveTo(QPointF(x, y)); }
	void lineTo(const QPointF &point);
	void lineTo(double x, double y) { lineTo(QPointF(x, y)); }
	
	QList<QPolygonF> polygons() const { return _polygons; }
	
private:
	
	QList<QPolygonF> _polygons;
};

#endif // MLSEGMENTPATH_H
