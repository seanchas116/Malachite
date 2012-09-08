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
					auto wrapped = MLGenericImage<ML::ImageFormatRgb, MLVec3U8>::wrap(srcBits, srcSize, srcPitch);
					dst->template paste<false, true>(wrapped, pos);
					break;
				}
				case 32:
				{
					dst->template paste<false, true>(MLGenericImage<ML::ImageFormatArgb, MLVec4U8>::wrap(srcBits, srcSize, srcPitch), pos);
					break;
				}
				default:
				{
					FIBITMAP *newBitmap = FreeImage_ConvertTo32Bits(src);	// converted to RGBA8
					dst->template paste<false, true>(MLGenericImage<ML::ImageFormatArgb, MLVec4U8>::wrap(FreeImage_GetBits(newBitmap), srcSize, FreeImage_GetPitch(newBitmap)), pos);
					FreeImage_Unload(newBitmap);
					break;
				}
			}
			
			break;
		}
		case FIT_RGB16:
		{
			dst->template paste<false, true>(MLGenericImage<ML::ImageFormatRgb, MLVec3U16>::wrap(srcBits, srcSize, srcPitch), pos);
			break;
		}
		case FIT_RGBA16:
		{
			dst->template paste<false, true>(MLGenericImage<ML::ImageFormatArgb, MLVec4U16>::wrap(srcBits, srcSize, srcPitch), pos);
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
					wrapper.paste<true, false>(src, pos);
					break;
				}
				case 32:
				{
					auto wrapper = MLGenericImage<ML::ImageFormatArgb, MLVec4U8>::wrap(dstBits, dstSize, dstPitch);
					wrapper.paste<true, false>(src, pos);
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
			wrapper.paste<true, false>(src, pos);
			break;
		}
		case FIT_RGBA16:
		{
			auto wrapper = MLGenericImage<ML::ImageFormatArgb, MLVec4U16>::wrap(dstBits, dstSize, dstPitch);
			wrapper.paste<true, false>(src, pos);
			break;
		}
		default:
			qWarning() << Q_FUNC_INFO << ": Unsupported data type";
			return false;
	}
	
	return true;
}

#endif // MLIMAGEIOPRIVATE_H
