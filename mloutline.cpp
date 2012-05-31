#include <cmath>
#include <QVector>

#include "mloutline.h"

void MLOutline::lineTo(float x, float y)
{
	float sx = x / y;
	float sy = y / x;
	
	float fx = floor(x);
	float fy = floor(y);
	
	int xc = fx - floor(_px);
	int yc = fy - floor(_py);
	
	QVector<MLPoint> xip(xc), yip(yc);
	
	float cy = y + (fx + 1 - x) * sy;
	float cx = x + (fy + 1 - y) * sx;
	
	for (int i = 0; i < xc; ++i)
	{
		cy += sy;
		xip[i] = MLPoint(fx + i, cy);
	}
	
	for (int i = 0; i < xc; ++i)
	{
		cx += sx;
		yip[i] = MLPoint(cx, fy + i);
	}
	
	QVector<MLPoint> ip;
}

void MLOutline::createCells(float x1, float y1, float x2, float y2)
{
	
}

