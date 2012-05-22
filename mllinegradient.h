#ifndef MLLINEGRADIENT_H
#define MLLINEGRADIENT_H

#include <QMap>
#include <QPointF>
#include "mlgradient.h"

class MALACHITESHARED_EXPORT MLLineGradient : public MLGradient
{
public:
	MLLineGradient() {}
	
	void addStop(float x, float y) { _stops.insert(x, y); }
	void addStop(const QPointF &point) { addStop(point.x(), point.y()); }
	void clear() { _stops.clear(); }
	
	float evaluate(float value) const;
	
private:
	QMap<float, float> _stops;
};

#endif // MLLINEGRADIENT_H
