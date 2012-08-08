#ifndef MLPIXELCONVERSION_H
#define MLPIXELCONVERSION_H

#include "mlglobal.h"
#include "mlvector.h"
#include <QDebug>

template <ML::ImageFormat DstFormat, class DstColor, ML::ImageFormat SrcFormat, class SrcColor>
void mlConvertPixel(DstColor &dst, const SrcColor &src)
{
	Q_UNUSED(dst); Q_UNUSED(src);
	qWarning() << "uninplemented image format / type conversion";
}

// 8bit unsigned rgb -> 32bit float argb premultiplied
template < >
void mlConvertPixel<ML::ImageFormatArgbFast, MLVec4F, ML::ImageFormatRgb, MLVec3U8>(MLVec4F &dst, const MLVec3U8 &src)
{
	dst.a = 0xFF;
	dst.r = src.r;
	dst.g = src.g;
	dst.b = src.b;
	dst *= 1.0 / 0xFF;
}

// 16bit unsigned rgb -> 32bit float argb premultiplied
template < >
void mlConvertPixel<ML::ImageFormatArgbFast, MLVec4F, ML::ImageFormatRgb, MLVec3U16>(MLVec4F &dst, const MLVec3U16 &src)
{
	dst.a = 0xFFFF;
	dst.r = src.r;
	dst.g = src.g;
	dst.b = src.b;
	dst *= 1.0 / 0xFFFF;
}

// 8bit unsigned argb -> 32bit float argb premultiplied
template < >
void mlConvertPixel<ML::ImageFormatArgbFast, MLVec4F, ML::ImageFormatArgb, MLVec4U8>(MLVec4F &dst, const MLVec4U8 &src)
{
	dst.a = src.a;
	dst.r = src.r;
	dst.g = src.g;
	dst.b = src.b;
	dst *= 1.0 / 0xFF;
	dst.r *= dst.a;
	dst.g *= dst.a;
	dst.b *= dst.a;
}

// 16bit unsigned argb -> 32bit float argb premultiplied
template < >
void mlConvertPixel<ML::ImageFormatArgbFast, MLVec4F, ML::ImageFormatArgb, MLVec4U16>(MLVec4F &dst, const MLVec4U16 &src)
{
	dst.a = src.a;
	dst.r = src.r;
	dst.g = src.g;
	dst.b = src.b;
	dst *= 1.0 / 0xFFFF;
	dst.r *= dst.a;
	dst.g *= dst.a;
	dst.b *= dst.a;
}

// 8bit unsigned argb premultiplied -> 32bit float argb premultiplied
template < >
void mlConvertPixel<ML::ImageFormatArgbFast, MLVec4F, ML::ImageFormatArgbFast, MLVec4U8>(MLVec4F &dst, const MLVec4U8 &src)
{
	dst.a = src.a;
	dst.r = src.r;
	dst.g = src.g;
	dst.b = src.b;
	dst *= 1.0 / 0xFF;
}

// 16bit unsigned argb premultiplied -> 32bit float argb premultiplied
template < >
void mlConvertPixel<ML::ImageFormatArgbFast, MLVec4F, ML::ImageFormatArgbFast, MLVec4U16>(MLVec4F &dst, const MLVec4U16 &src)
{
	dst.a = src.a;
	dst.r = src.r;
	dst.g = src.g;
	dst.b = src.b;
	dst *= 1.0 / 0xFFFF;
}

// 32bit float argb premultiplied -> 8bit unsigned rgb
template < >
void mlConvertPixel<ML::ImageFormatRgb, MLVec3U8, ML::ImageFormatArgbFast, MLVec4F>(MLVec3U8 &dst, const MLVec4F &src)
{
	MLVec4F v = src * 0xFF;
	MLVec4I32 iv = mlRound(v);
	
	dst.r = iv.r;
	dst.g = iv.g;
	dst.b = iv.b;
}

// 32bit float argb premultiplied -> 16bit unsigned rgb
template < >
void mlConvertPixel<ML::ImageFormatRgb, MLVec3U16, ML::ImageFormatArgbFast, MLVec4F>(MLVec3U16 &dst, const MLVec4F &src)
{
	MLVec4F v = src * 0xFFFF;
	MLVec4I32 iv = mlRound(v);
	
	dst.r = iv.r;
	dst.g = iv.g;
	dst.b = iv.b;
}

// 32bit float argb premultiplied -> 8bit unsigned argb
template < >
void mlConvertPixel<ML::ImageFormatArgb, MLVec4U8, ML::ImageFormatArgbFast, MLVec4F>(MLVec4U8 &dst, const MLVec4F &src)
{
	MLVec4F v = src;
	v.r *= v.a;
	v.g *= v.a;
	v.b *= v.a;
	
	v *= 0xFF;
	
	MLVec4I32 iv = mlRound(v);
	
	dst.a = iv.a;
	dst.r = iv.r;
	dst.g = iv.g;
	dst.b = iv.b;
}

// 32bit float argb premultiplied -> 16bit unsigned argb
template < >
void mlConvertPixel<ML::ImageFormatArgb, MLVec4U16, ML::ImageFormatArgbFast, MLVec4F>(MLVec4U16 &dst, const MLVec4F &src)
{
	MLVec4F v = src;
	v.r *= v.a;
	v.g *= v.a;
	v.b *= v.a;
	
	v *= 0xFFFF;
	
	MLVec4I32 iv = mlRound(v);
	
	dst.a = iv.a;
	dst.r = iv.r;
	dst.g = iv.g;
	dst.b = iv.b;
}

// 32bit float argb premultiplied -> 8bit unsigned argb premultiplied
template < >
void mlConvertPixel<ML::ImageFormatArgbFast, MLVec4U8, ML::ImageFormatArgbFast, MLVec4F>(MLVec4U8 &dst, const MLVec4F &src)
{
	MLVec4F v = src * 0xFF;
	MLVec4I32 iv = mlRound(v);
	
	dst.a = iv.a;
	dst.r = iv.r;
	dst.g = iv.g;
	dst.b = iv.b;
}

// 32bit float argb premultiplied -> 16bit unsigned argb premultiplied
template < >
void mlConvertPixel<ML::ImageFormatArgbFast, MLVec4U16, ML::ImageFormatArgbFast, MLVec4F>(MLVec4U16 &dst, const MLVec4F &src)
{
	MLVec4F v = src * 0xFFFF;
	MLVec4I32 iv = mlRound(v);
	
	dst.a = iv.a;
	dst.r = iv.r;
	dst.g = iv.g;
	dst.b = iv.b;
}


#endif // MLPIXELCONVERSION_H
