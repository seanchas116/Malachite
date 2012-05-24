#ifndef MLIMAGEPAINTENGINE_H
#define MLIMAGEPAINTENGINE_H

#include "mlpaintengine.h"
#include "mlrasterizer.h"

class MLImagePaintEngine : public MLPaintEngine
{
public: 
	
	MLImagePaintEngine();
	
	bool begin(MLPaintable *paintable);
	bool flush();
	
	void updateState(const MLPaintEngineState &state);
	
	void drawPath(const QPainterPath &path);
	void drawImage(const QPoint &point, const MLImage &image);
	
	QRect rect() const { return QRect(QPoint(), _size); }
	
	void fillScanline(int x, int y, int count, float *covers);
	void fillScanline(int x, int y, int count);
	
	MLFastArgbF *pixelPointer(int x, int y)
	{
		return reinterpret_cast<MLFastArgbF *>(_bits + _bytesPerLine * y) + x;
	}
	
	MLFastArgbF *pixelPointer(const QPointF &p) { return pixelPointer(p.x(), p.y()); }
	
private:
	
	MLRasterizer<MLImagePaintEngine> _rasterizer;
	
	MLBlendOp *_blendOp;
	float _opacity;
	
	uint8_t *_bits;
	QSize _size;
	int _bytesPerLine;
	
	MLPaintEngineState _state;
	MLImage *_image;
};

#endif // MLIMAGEPAINTENGINE_H
