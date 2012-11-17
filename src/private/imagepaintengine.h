#ifndef MLIMAGEPAINTENGINE_H
#define MLIMAGEPAINTENGINE_H

#include "../paintengine.h"
#include "renderer.h"

namespace Malachite
{

class ImagePaintEngine : public PaintEngine
{
public: 
	
	ImagePaintEngine();
	
	bool begin(Paintable *paintable);
	bool flush();
	
	void drawTransformedPolygons(const FixedMultiPolygon &polygons);
	void drawTransformedImage(const QPoint &point, const Image &image, const QRect &imageMaskRect);
	
private:
	
	ArgbBitmap _bitmap;
	Image *_image;
};

}

#endif // MLIMAGEPAINTENGINE_H
