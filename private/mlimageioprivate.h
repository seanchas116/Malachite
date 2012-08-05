#ifndef MLIMAGEIOPRIVATE_H
#define MLIMAGEIOPRIVATE_H

#include <FreeImage.h>
#include <QPoint>
#include <QSize>

template <typename Image>
bool mlPasteFIBITMAPToImage(const QPoint &p, FIBITMAP *src, Image *dst)
{
	FREE_IMAGE_TYPE type = FreeImage_GetImageType(src);
	
	QSize size(FreeImage_GetWidth(src), FreeImage_GetHeight(src));
	const uint8_t *srcBits = FreeImage_GetBits(src);
	int pitch = FreeImage_GetPitch(src);
	
	if (type == FIT_BITMAP) {
		int bpp = FreeImage_GetBPP(src);
		if (bpp == 24) {	// RGB8
			MLGenericConstWrapperImageM<MLRgb8> wrapper(srcBits, size, pitch);
			dst->paste(p, wrapper);
		} else if (bpp == 32) {	// ARGB8
			MLGenericConstWrapperImageM<MLArgb8> wrapper(srcBits, size, pitch);
			dst->paste(p, wrapper);
		} else {
			FIBITMAP *newBitmap = FreeImage_ConvertTo24Bits(src);
			MLGenericConstWrapperImageM<MLRgb8> wrapper(FreeImage_GetBits(newBitmap), size, FreeImage_GetPitch(newBitmap));
			dst->paste(p, wrapper);
			FreeImage_Unload(newBitmap);
		}
	} else if (type == FIT_RGB16) {
		MLGenericConstWrapperImageM<MLRgb16> wrapper(srcBits, size, pitch);
		dst->paste(p, wrapper);
	} else if (type == FIT_RGBA16) {
		MLGenericConstWrapperImageM<MLArgb16> wrapper(srcBits, size, pitch);
		dst->paste(p, wrapper);
	} else {
		qDebug() << "MLImage::fromFIBITMAP: Unsupported data type";
		return false;
	}
	return true;
}

#endif // MLIMAGEIOPRIVATE_H
