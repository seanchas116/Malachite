#ifndef MLIMAGEPAINTENGINE_H
#define MLIMAGEPAINTENGINE_H

#include "mlpaintengine.h"
#include "mlrenderer.h"

class MLImagePaintEngine : public MLPaintEngine
{
public: 
	
	MLImagePaintEngine();
	
	bool begin(MLPaintable *paintable);
	bool flush();
	
	void drawTransformedPolygons(const MLFixedMultiPolygon &polygons);
	void drawTransformedImage(const QPoint &point, const MLImage &image);
	
private:
	
	MLArgbBitmap _bitmap;
	MLImage *_image;
};


#endif // MLIMAGEPAINTENGINE_H
