#ifndef MLIMAGEIOPRIVATE_H
#define MLIMAGEIOPRIVATE_H

#include "mlgenericimage.h"
#include <FreeImage.h>
#include <QPoint>
#include <QSize>

template <typename Image>
bool mlPasteFIBITMAPToImage(const QPoint &p, Image *dst, FIBITMAP *src)
{
	FREE_IMAGE_TYPE type = FreeImage_GetImageType(src);
	
	QSize size(FreeImage_GetWidth(src), FreeImage_GetHeight(src));
	const uint8_t *srcBits = FreeImage_GetBits(src);
	int pitch = FreeImage_GetPitch(src);
	
	if (type == FIT_BITMAP) {
		int bpp = FreeImage_GetBPP(src);
		if (bpp == 24) {	// RGB8
			dst->paste(MLGenericImage<ML::ImageFormatRgb, MLVec3U8>::wrap(srcBits, size, pitch), true, p);
		} else if (bpp == 32) {	// ARGB8
			dst->paste(MLGenericImage<ML::ImageFormatArgb, MLVec4U8>::wrap(srcBits, size, pitch), true, p);
		} else {
			FIBITMAP *newBitmap = FreeImage_ConvertTo24Bits(src);	// converted to RGB8
			dst->paste(MLGenericImage<ML::ImageFormatRgb, MLVec3U8>::wrap(FreeImage_GetBits(newBitmap), size, FreeImage_GetPitch(newBitmap)), true, p);
			FreeImage_Unload(newBitmap);
		}
	} else if (type == FIT_RGB16) {
		dst->paste(MLGenericImage<ML::ImageFormatRgb, MLVec3U16>::wrap(srcBits, size, pitch), true, p);
	} else if (type == FIT_RGBA16) {
		dst->paste(MLGenericImage<ML::ImageFormatArgb, MLVec4U16>::wrap(srcBits, size, pitch), true, p);
	} else {
		qDebug() << "MLImage::fromFIBITMAP: Unsupported data type";
		return false;
	}
	return true;
}

#endif // MLIMAGEIOPRIVATE_H
