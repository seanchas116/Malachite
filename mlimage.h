#ifndef MLIMAGE_H
#define MLIMAGE_H

#include "mlpaintable.h"
#include "mlcolor.h"
#include "mlgenericimage.h"

namespace Malachite
{

typedef GenericImage<Malachite::ImageFormatArgbFast, Vec4U8> MLImage32;

class MALACHITESHARED_EXPORT Image : public GenericImage<Malachite::ImageFormatArgbFast, Vec4F>, public Paintable
{
public:
	Image() : GenericImage<Malachite::ImageFormatArgbFast, Vec4F>() {}
	Image(const GenericImage<Malachite::ImageFormatArgbFast, Vec4F> &other) : GenericImage<Malachite::ImageFormatArgbFast, Vec4F>(other) {}
	Image(const QSize &size) : GenericImage<Malachite::ImageFormatArgbFast, Vec4F>(size) {}
	Image(int width, int height) : GenericImage<Malachite::ImageFormatArgbFast, Vec4F>(width, height) {}
	
	void clear() { fill(Vec4F(0)); }
	
	bool isBlank() const;
	
	//MLArgb colorSummation() const;
	Vec4F colorSummation(const QPoint &maskOffset, const Image &mask) const;
	
	PaintEngine *createPaintEngine();
	
	Image toOpaqueImage() const;
	
	QImage toQImage() const;
	
	static Image fromQImage(const QImage &qimage);
	
	Image &operator*=(float factor);
};

inline Image operator*(const Image &image, float factor)
{
	Image result = image;
	result *= factor;
	return result;
}

}

Q_DECLARE_METATYPE(Malachite::Image)

#endif // MLIMAGE_H
