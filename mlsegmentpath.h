#ifndef MLSEGMENTPATH_H
#define MLSEGMENTPATH_H

#include <QPainterPath>
#include "mlglobal.h"
#include "mlpolygon.h"

class MALACHITESHARED_EXPORT MLSegmentPath
{
public:
	MLSegmentPath() {}
	MLSegmentPath(const QPainterPath &path);
	MLSegmentPath(const QPolygonF &polygon);
	MLSegmentPath(const MLPolygon &polygon);
	
	void moveTo(const MLPoint &point);
	void moveTo(float x, float y) { moveTo(MLPoint(x, y)); }
	void lineTo(const MLPoint &point);
	void lineTo(float x, float y) { lineTo(MLPoint(x, y)); }
	
	QList<MLPolygon> polygons() const { return _polygons; }
	
private:
	
	QList<MLPolygon> _polygons;
};

#endif // MLSEGMENTPATH_H
