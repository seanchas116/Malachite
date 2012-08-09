#ifndef MLIMAGE_H
#define MLIMAGE_H

#include "mlpaintable.h"
#include "mlcolor.h"
#include "mlgenericimage.h"

typedef MLGenericImage<ML::ImageFormatArgbFast, MLVec4U8> MLImage32;

class MALACHITESHARED_EXPORT MLImage : public MLGenericImage<ML::ImageFormatArgbFast, MLVec4F>, public MLPaintable
{
public:
	MLImage() : MLGenericImage<ML::ImageFormatArgbFast, MLVec4F>() {}
	MLImage(const MLGenericImage<ML::ImageFormatArgbFast, MLVec4F> &other) : MLGenericImage<ML::ImageFormatArgbFast, MLVec4F>(other) {}
	MLImage(const QSize &size) : MLGenericImage<ML::ImageFormatArgbFast, MLVec4F>(size) {}
	MLImage(int width, int height) : MLGenericImage<ML::ImageFormatArgbFast, MLVec4F>(width, height) {}
	
	void clear() { fill(MLVec4F(0)); }
	
	bool isBlank() const;
	
	//MLArgb colorSummation() const;
	MLVec4F colorSummation(const QPoint &maskOffset, const MLImage &mask) const;
	
	MLPaintEngine *createPaintEngine();
	
	QImage toQImage() const;
	
	static MLImage fromQImage(const QImage &qimage);
};

Q_DECLARE_METATYPE(MLImage)



#endif // MLIMAGE_H
