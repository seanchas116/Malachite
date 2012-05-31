#ifndef MLOUTLINE_H
#define MLOUTLINE_H

#include "mlpoint.h"
#include <QSet>

struct MLCell
{
	MLCell(int x, int y, float cover) : x(x), y(y), cover(cover) {}
	
	int x, y;
	float cover;
};

class MLOutline
{
public:
	MLOutline() :
		_px(0),
		_py(0)
	{}
	
	void moveTo(float x, float y)
	{
		_px = x; _py = y;
	}

	void lineTo(float x, float y);
	
	void createCells(float x1, float y1, float x2, float y2);
	
private:
	
	QSet<MLCell> _cells;
	float _px, _py;
};

#endif // MLOUTLINE_H
