#ifndef MLIMAGEPAINTENGINE_H
#define MLIMAGEPAINTENGINE_H

#include "mlpaintengine.h"

class MLImagePaintEngine : public MLPaintEngine
{
public: 
	
	MLImagePaintEngine() : MLPaintEngine() {}
	
	bool begin(MLPaintable *paintable);
	bool flush();
	
	void updateState(const MLPaintEngineState &state);
	
	void drawPath(const QPainterPath &path);
	void drawImage(const QPoint &point, const MLImage &image);
	
private:
	
	MLBlendOp *_blendOp;
	float _opacity;
	
	MLBitmap<MLFastArgbF> _bitmap;
	
	MLPaintEngineState _state;
	MLImage *_image;
};

#endif // MLIMAGEPAINTENGINE_H
