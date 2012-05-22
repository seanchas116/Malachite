#ifndef MLCURVES_H
#define MLCURVES_H

#include <QPolygonF>
#include "mlglobal.h"

class MALACHITESHARED_EXPORT MLCurve4
{
public:
	MLCurve4() {}
	MLCurve4(const QPointF &start, const QPointF &control1, const QPointF &control2, const QPointF &end)
	{
		this->start = start;
		this->control1 = control1;
		this->control2 = control2;
		this->end = end;
	}
	
	static MLCurve4 fromCatmullRom(const QPointF &previous, const QPointF &start, const QPointF &end, const QPointF &next)
	{
		return MLCurve4(start, start + (end - previous) / 6.0, end - (next - start) / 6.0, end);
	}
	
	static MLCurve4 fromCatmullRomStart(const QPointF &start, const QPointF &end, const QPointF &next)
	{
		return MLCurve4(start, start + (end - start) / 3.0, end - (next - start) / 6.0, end);
	}
	
	static MLCurve4 fromCatmullRomEnd(const QPointF &previous, const QPointF &start, const QPointF &end)
	{
		return MLCurve4(start, start + (end - previous) / 6.0, (end - start) / 3.0, end);
	}
	
	static MLCurve4 fromCatmullRom(const QPolygonF &polygon, int indexStart);
	
	QPointF start, control1, control2, end;
};

#endif // MLCURVES_H
