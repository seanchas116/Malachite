#ifndef MLRASTERIZER_H
#define MLRASTERIZER_H

#include <QPolygonF>
#include "mlsegmentpath.h"
#include "mlmisc.h"

// simple polygon rasterizer
// using arithmetic antialiasing method
template <typename Renderer>
class MLRasterizer
{
public:
	
	MLRasterizer(Renderer *renderer) : _renderer(renderer) {}
	
	void rasterize(const QPainterPath &path);
	
private:
	
	Renderer *_renderer;
};

template <typename Renderer>
void MLRasterizer<Renderer>::rasterize(const QPainterPath &path)
{
	QRect rect = path.boundingRect().toAlignedRect() & _renderer->rect();
	
	for (int ry = 0; ry < rect.height(); ++ry)
	{
		int y = rect.top() + ry;
		
		QPainterPath strip;
		strip.addRect(rect.left(), y, rect.width(), 1);
		strip &= path;
		
		QRect stripRect = strip.boundingRect().toAlignedRect();
		
		float *covers = new float[stripRect.width()];
		
		for (int rx = 0; rx < stripRect.width(); ++rx)
		{
			int x = stripRect.left() + rx;
			QPainterPath square;
			square.addRect(x, y, 1, 1);
			covers[rx] = mlCalculatePolygonSpace((strip & square).toFillPolygon());
		}
		
		_renderer->fillScanline(stripRect.left(), y, stripRect.width(), covers);
		
		delete [] covers;
	}
}


#endif // MLRASTERIZER_H
