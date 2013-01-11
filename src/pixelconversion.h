#ifndef MLPIXELCONVERSION_H
#define MLPIXELCONVERSION_H

//ExportName: PixelConversion

#include "global.h"
#include "vector.h"
#include <QDebug>

namespace Malachite
{

template <ImageFormat DstFormat, class DstColor, ImageFormat SrcFormat, class SrcColor>
inline void convertPixel(DstColor &dst, const SrcColor &src)
{
	Q_UNUSED(dst); Q_UNUSED(src);
	qWarning() << "uninplemented image format / type conversion";
}

// 8bit unsigned rgb -> 32bit float argb premultiplied
template < >
inline void convertPixel<ImageFormatArgbFast, Vec4F, ImageFormatRgb, Vec3U8>(Vec4F &dst, const Vec3U8 &src)
{
	dst.a = 0xFF;
	dst.r = src.r;
	dst.g = src.g;
	dst.b = src.b;
	dst *= 1.0 / 0xFF;
}

// 16bit unsigned rgb -> 32bit float argb premultiplied
template < >
inline void convertPixel<ImageFormatArgbFast, Vec4F, ImageFormatRgb, Vec3U16>(Vec4F &dst, const Vec3U16 &src)
{
	dst.a = 0xFFFF;
	dst.r = src.r;
	dst.g = src.g;
	dst.b = src.b;
	dst *= 1.0 / 0xFFFF;
}

// 8bit unsigned argb -> 32bit float argb premultiplied
template < >
inline void convertPixel<ImageFormatArgbFast, Vec4F, ImageFormatArgb, Vec4U8>(Vec4F &dst, const Vec4U8 &src)
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
inline void convertPixel<ImageFormatArgbFast, Vec4F, ImageFormatArgb, Vec4U16>(Vec4F &dst, const Vec4U16 &src)
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
inline void convertPixel<ImageFormatArgbFast, Vec4F, ImageFormatArgbFast, Vec4U8>(Vec4F &dst, const Vec4U8 &src)
{
	dst.a = src.a;
	dst.r = src.r;
	dst.g = src.g;
	dst.b = src.b;
	dst *= 1.0 / 0xFF;
}

// 16bit unsigned argb premultiplied -> 32bit float argb premultiplied
template < >
inline void convertPixel<ImageFormatArgbFast, Vec4F, ImageFormatArgbFast, Vec4U16>(Vec4F &dst, const Vec4U16 &src)
{
	dst.a = src.a;
	dst.r = src.r;
	dst.g = src.g;
	dst.b = src.b;
	dst *= 1.0 / 0xFFFF;
}

// 32bit float argb premultiplied -> 8bit unsigned rgb
template < >
inline void convertPixel<ImageFormatRgb, Vec3U8, ImageFormatArgbFast, Vec4F>(Vec3U8 &dst, const Vec4F &src)
{
	Vec4F v = src + (Vec4F(1.f) - src.extract3()) * Vec4F(1.f);
	v *= 0xFF;
	Vec4I32 iv = vecRound(v);
	
	dst.r = iv.r;
	dst.g = iv.g;
	dst.b = iv.b;
}

// 32bit float argb premultiplied -> 16bit unsigned rgb
template < >
inline void convertPixel<ImageFormatRgb, Vec3U16, ImageFormatArgbFast, Vec4F>(Vec3U16 &dst, const Vec4F &src)
{
	Vec4F v = src + (Vec4F(1.f) - src.extract3()) * Vec4F(1.f);
	v *= 0xFFFF;
	Vec4I32 iv = vecRound(v);
	
	dst.r = iv.r;
	dst.g = iv.g;
	dst.b = iv.b;
}

// 32bit float argb premultiplied -> 8bit unsigned argb
template < >
inline void convertPixel<ImageFormatArgb, Vec4U8, ImageFormatArgbFast, Vec4F>(Vec4U8 &dst, const Vec4F &src)
{
	Vec4F v = src;
	
	float a = v.a;
	if (a)
	{
		v /= a;
		v.a = a;
	}
	
	v *= 0xFF;
	
	Vec4I32 iv = vecRound(v);
	dst.a = iv.a;
	dst.r = iv.r;
	dst.g = iv.g;
	dst.b = iv.b;
}

// 32bit float argb premultiplied -> 16bit unsigned argb
template < >
inline void convertPixel<ImageFormatArgb, Vec4U16, ImageFormatArgbFast, Vec4F>(Vec4U16 &dst, const Vec4F &src)
{
	Vec4F v = src;
	
	float a = v.a;
	if (a)
	{
		v /= a;
		v.a = a;
	}
	
	v *= 0xFFFF;
	
	Vec4I32 iv = vecRound(v);
	
	dst.a = iv.a;
	dst.r = iv.r;
	dst.g = iv.g;
	dst.b = iv.b;
}

// 32bit float argb premultiplied -> 16bit unsigned bgra
template < >
inline void convertPixel<ImageFormatBgra, Vec4U16, ImageFormatArgbFast, Vec4F>(Vec4U16 &dst, const Vec4F &src)
{
	Vec4F v = src;
	
	float a = v.a;
	if (a)
	{
		v /= a;
		v.a = a;
	}
	
	v *= 0xFFFF;
	
	Vec4I32 iv = vecRound(v);
	
	dst.a = iv.a;
	dst.b = iv.r;
	dst.g = iv.g;
	dst.r = iv.b;
}

// 32bit float argb premultiplied -> 8bit unsigned argb premultiplied
template < >
inline void convertPixel<ImageFormatArgbFast, Vec4U8, ImageFormatArgbFast, Vec4F>(Vec4U8 &dst, const Vec4F &src)
{
	Vec4F v = src * 0xFF;
	Vec4I32 iv = vecRound(v);
	
	dst.a = iv.a;
	dst.r = iv.r;
	dst.g = iv.g;
	dst.b = iv.b;
}

// 32bit float argb premultiplied -> 16bit unsigned argb premultiplied
template < >
inline void convertPixel<ImageFormatArgbFast, Vec4U16, ImageFormatArgbFast, Vec4F>(Vec4U16 &dst, const Vec4F &src)
{
	Vec4F v = src * 0xFFFF;
	Vec4I32 iv = vecRound(v);
	
	dst.a = iv.a;
	dst.r = iv.r;
	dst.g = iv.g;
	dst.b = iv.b;
}

}


#endif // MLPIXELCONVERSION_H
