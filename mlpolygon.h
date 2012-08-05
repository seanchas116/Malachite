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
	
	QPolygonF toQPolygonF() const;
};

#endif // MLPOLYGON_H
