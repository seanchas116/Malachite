#include <QtCore>
#include <FreeImage.h>

#include "private/mlimagepaintengine.h"
#include "mlgenericwrapperimage.h"
#include "mlimage.h"

bool MLImage::isBlank() const
{
	for (int y = 0; y < height(); ++y) {
		const MLArgb *p = constScanline(y);
		for (int x = 0; x < width(); ++x) {
			if (p->a())
				return false;
			p++;
		}
	}
	return true;
}

MLPaintEngine *MLImage::createPaintEngine()
{
	return new MLImagePaintEngine();
}

QImage MLImage::toQImage() const
{
	QImage image(size(), QImage::Format_ARGB32_Premultiplied);
	MLGenericWrapperImage<MLFastArgb8> wrapper(image.bits(), image.size(), image.bytesPerLine());
	wrapper.paste(*this);
	return image;
}

MLImage MLImage::fromQImage(const QImage &qimage)
{
	MLImage image(qimage.size());
	MLGenericConstWrapperImage<MLFastArgb8> wrapper(qimage.bits(), qimage.size(), qimage.bytesPerLine());
	image.paste(wrapper);
	return image;
}


