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
	
	template <class Rasterizer, class Filler> void fill(Rasterizer &ras, Filler &filler);
	
	template <class Rasterizer> void fillImage(Rasterizer &ras, const MLImage &image, MLGlobal::SpreadType spreadType, const QTransform &transform);
	template <class Rasterizer, class Gradient, class Method> void fillGradient(Rasterizer &ras, Gradient *gradient, const Method &method, MLGlobal::SpreadType spreadType, const QTransform &transform);
	template <class Rasterizer, class Gradient, class Method> void fillGradient(Rasterizer &ras, Gradient *gradient, const Method &method, MLGlobal::SpreadType spreadType);
	
	MLBlendOp *_blendOp;
	float _opacity;
	
	MLBitmap<MLArgb> _bitmap;
	
	MLPaintEngineState _state;
	MLImage *_image;
};

#endif // MLIMAGEPAINTENGINE_H
