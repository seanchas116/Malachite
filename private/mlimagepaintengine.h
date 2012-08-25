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
	
	void updateState(const MLPaintEngineState &state);
	
	void drawPath(const QPainterPath &path);
	void drawImage(const QPoint &point, const MLImage &image);
	
private:
	
	void drawImageSimple(const QPoint &offset, const MLImage &image);
	
	MLBlendOp *_blendOp;
	
	MLArgbBitmap _bitmap;
	
	MLPaintEngineState _state;
	MLImage *_image;
};


#endif // MLIMAGEPAINTENGINE_H
