#ifndef MLIMAGEPAINTENGINE_H
#define MLIMAGEPAINTENGINE_H

#include "mlpaintengine.h"

class MLFiller;

class MLImagePaintEngine : public MLPaintEngine
{
public: 
	
	MLImagePaintEngine() : MLPaintEngine() {}
	
	bool begin(MLPaintable *paintable);
	bool flush();
	
	void updateState(const MLPaintEngineState &state);
	
	//template <class Rasterizer, class Filler> inline void fill(Rasterizer &ras, Filler &filler);
	//template <MLGlobal::SpreadType SpreadType> void drawPathReal(const QPainterPath &path);
	void drawPath(const QPainterPath &path);
	void drawImage(const QPoint &point, const MLImage &image);
	
	template <class Rasterizer> void fill(const Rasterizer &ras, const MLFiller &filler);
	
private:
	
	MLBlendOp *_blendOp;
	float _opacity;
	
	MLBitmap<MLArgb> _bitmap;
	
	MLPaintEngineState _state;
	MLImage *_image;
};

#endif // MLIMAGEPAINTENGINE_H
