#ifndef MLRASTERIZER_H
#define MLRASTERIZER_H

#include <QPolygonF>
#include "mlsegmentpath.h"
#include "mlmisc.h"
#include "mlscanlineclipper.h"

// simple polygon rasterizer
// using arithmetic antialiasing method
template <typename Renderer>
class MLRasterizer
{
public:
	
	MLRasterizer(Renderer *renderer) : _renderer(renderer) {}
	
	void rasterize(const MLPolygon &polygon);
	
private:
	
	Renderer *_renderer;
};

template <typename Renderer>
void MLRasterizer<Renderer>::rasterize(const MLPolygon &polygon)
{
	MLScanlineClipper<false> _clipperY(polygon);
	
	for (int ry = 0; ry < _clipperY.scanlineCount(); ++ry)
	{
		int y = _clipperY.scanlineMin() + ry;
		
		MLPolygon strip = _clipperY.clipNext();
		MLScanlineClipper<true> _clipperX(strip);
		
		float *covers = new float[_clipperX.scanlineCount()];
		
		for (int rx = 0; rx < _clipperX.scanlineCount(); ++rx)
		{
			MLPolygon frag = _clipperX.clipNext();
			covers[rx] = mlCalculatePolygonSpace(frag);
		}
		
		_renderer->fillScanline(_clipperX.scanlineMin(), _clipperY.scanlineMin() + ry, _clipperX.scanlineCount(), covers);
		
		delete [] covers;
	}
}


#endif // MLRASTERIZER_H
