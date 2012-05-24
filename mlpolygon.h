#ifndef MLPOLYGON_H
#define MLPOLYGON_H

#include <QPolygonF>
#include "mlpoint.h"

class MLPolygon : QVector<MLPoint>
{
public:
	MLPolygon() : QVector<MLPoint>() {}
	MLPolygon(int size) : QVector<MLPoint>(size) {}
	MLPolygon(const QVector<MLPoint> &v) : QVector<MLPoint>(y) {}
	MLPolygon(const QPolygonF &polygon);
};

#endif // MLPOLYGON_H
