#ifndef MLIMAGEIOPRIVATE_H
#define MLIMAGEIOPRIVATE_H

#include "mlgenericimage.h"
#include <FreeImage.h>
#include <QPoint>
#include <QSize>

template <class Image>
bool mlPasteFIBITMAPToImage(const QPoint &pos, Image *dst, FIBITMAP *src)
{
	FREE_IMAGE_TYPE srcType = FreeImage_GetImageType(src);
	QSize srcSize(FreeImage_GetWidth(src), FreeImage_GetHeight(src));
	const uint8_t *srcBits = FreeImage_GetBits(src);
	int srcPitch = FreeImage_GetPitch(src);
	
	switch (srcType)
	{
	case FIT_BITMAP:
	{
		int bpp = FreeImage_GetBPP(src);
		
		switch (bpp)
		{
		case 24:
		{
			dst->paste(MLGenericImage<ML::ImageFormatRgb, MLVec3U8>::wrap(srcBits, srcSize, srcPitch), pos, false, true);
			break;
		}
		case 32:
		{
			dst->paste(MLGenericImage<ML::ImageFormatArgb, MLVec4U8>::wrap(srcBits, srcSize, srcPitch), pos, false, true);
			break;
		}
		default:
		{
			FIBITMAP *newBitmap = FreeImage_ConvertTo32Bits(src);	// converted to RGBA8
			dst->paste(MLGenericImage<ML::ImageFormatArgb, MLVec4U8>::wrap(FreeImage_GetBits(newBitmap), srcSize, FreeImage_GetPitch(newBitmap)), pos, false, true);
			FreeImage_Unload(newBitmap);
			break;
		}
		}
		
		break;
	}
	case FIT_RGB16:
	{
		dst->paste(MLGenericImage<ML::ImageFormatRgb, MLVec3U16>::wrap(srcBits, srcSize, srcPitch), pos, false, true);
		break;
	}
	case FIT_RGBA16:
	{
		dst->paste(MLGenericImage<ML::ImageFormatArgb, MLVec4U16>::wrap(srcBits, srcSize, srcPitch), pos, false, true);
		break;
	}
	default:
		qWarning() << Q_FUNC_INFO << ": Unsupported data type";
		return false;
	}
	
	return true;
}

template <class Image>
bool mlPasteImageToBitmap(const QPoint &pos, FIBITMAP *dst, const Image &src)
{
	FREE_IMAGE_TYPE dstType = FreeImage_GetImageType(dst);
	QSize dstSize(FreeImage_GetWidth(dst), FreeImage_GetHeight(dst));
	int dstPitch = FreeImage_GetPitch(dst);
	uint8_t *dstBits = FreeImage_GetBits(dst);
	
	switch (dstType)
	{
	case FIT_BITMAP:
	{
		int bpp = FreeImage_GetBPP(dst);
		
		switch (bpp)
		{
		case 24:
		{
			auto wrapper = MLGenericImage<ML::ImageFormatRgb, MLVec3U8>::wrap(dstBits, dstSize, dstPitch);
			wrapper.paste(src, pos, true, false);
			break;
		}
		case 32:
		{
			auto wrapper = MLGenericImage<ML::ImageFormatArgb, MLVec4U8>::wrap(dstBits, dstSize, dstPitch);
			wrapper.paste(src, pos, true, false);
			break;
		}
		default:
			qWarning() << Q_FUNC_INFO << ": Unsupported data type";
			return false;
		}
		
		break;
	}
	case FIT_RGB16:
	{
		auto wrapper = MLGenericImage<ML::ImageFormatRgb, MLVec3U16>::wrap(dstBits, dstSize, dstPitch);
		wrapper.paste(src, pos, true, false);
		break;
	}
	case FIT_RGBA16:
	{
		auto wrapper = MLGenericImage<ML::ImageFormatArgb, MLVec4U16>::wrap(dstBits, dstSize, dstPitch);
		wrapper.paste(src, pos, true, false);
		break;
	}
	default:
		qWarning() << Q_FUNC_INFO << ": Unsupported data type";
		return false;
	}
	
	return true;
}

#endif // MLIMAGEIOPRIVATE_H
