#ifndef MLIMAGEPAINTENGINEV2_H
#define MLIMAGEPAINTENGINEV2_H

#include "mlpaintengine.h"
#include "mlimagerendererv2.h"

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
	
	MLBlendOp *_blendOp;
	
	MLArgbBitmap _bitmap;
	
	MLPaintEngineState _state;
	MLImage *_image;
};


#endif // MLIMAGEPAINTENGINEV2_H
