#ifndef MLPIXELCONVERSION_H
#define MLPIXELCONVERSION_H

#include "mlglobal.h"
#include "mlvector.h"
#include <QDebug>

namespace Malachite
{

template <Malachite::ImageFormat DstFormat, class DstColor, Malachite::ImageFormat SrcFormat, class SrcColor>
inline void convertPixel(DstColor &dst, const SrcColor &src)
{
	Q_UNUSED(dst); Q_UNUSED(src);
	qWarning() << "uninplemented image format / type conversion";
}

// 8bit unsigned rgb -> 32bit float argb premultiplied
template < >
inline void convertPixel<Malachite::ImageFormatArgbFast, Vec4F, Malachite::ImageFormatRgb, Vec3U8>(Vec4F &dst, const Vec3U8 &src)
{
	dst.a = 0xFF;
	dst.r = src.r;
	dst.g = src.g;
	dst.b = src.b;
	dst *= 1.0 / 0xFF;
}

// 16bit unsigned rgb -> 32bit float argb premultiplied
template < >
inline void convertPixel<Malachite::ImageFormatArgbFast, Vec4F, Malachite::ImageFormatRgb, Vec3U16>(Vec4F &dst, const Vec3U16 &src)
{
	dst.a = 0xFFFF;
	dst.r = src.r;
	dst.g = src.g;
	dst.b = src.b;
	dst *= 1.0 / 0xFFFF;
}

// 8bit unsigned argb -> 32bit float argb premultiplied
template < >
inline void convertPixel<Malachite::ImageFormatArgbFast, Vec4F, Malachite::ImageFormatArgb, Vec4U8>(Vec4F &dst, const Vec4U8 &src)
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
inline void convertPixel<Malachite::ImageFormatArgbFast, Vec4F, Malachite::ImageFormatArgb, Vec4U16>(Vec4F &dst, const Vec4U16 &src)
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
inline void convertPixel<Malachite::ImageFormatArgbFast, Vec4F, Malachite::ImageFormatArgbFast, Vec4U8>(Vec4F &dst, const Vec4U8 &src)
{
	dst.a = src.a;
	dst.r = src.r;
	dst.g = src.g;
	dst.b = src.b;
	dst *= 1.0 / 0xFF;
}

// 16bit unsigned argb premultiplied -> 32bit float argb premultiplied
template < >
inline void convertPixel<Malachite::ImageFormatArgbFast, Vec4F, Malachite::ImageFormatArgbFast, Vec4U16>(Vec4F &dst, const Vec4U16 &src)
{
	dst.a = src.a;
	dst.r = src.r;
	dst.g = src.g;
	dst.b = src.b;
	dst *= 1.0 / 0xFFFF;
}

// 32bit float argb premultiplied -> 8bit unsigned rgb
template < >
inline void convertPixel<Malachite::ImageFormatRgb, Vec3U8, Malachite::ImageFormatArgbFast, Vec4F>(Vec3U8 &dst, const Vec4F &src)
{
	Vec4F v = src * 0xFF;
	Vec4I32 iv = vecRound(v);
	
	dst.r = iv.r;
	dst.g = iv.g;
	dst.b = iv.b;
}

// 32bit float argb premultiplied -> 16bit unsigned rgb
template < >
inline void convertPixel<Malachite::ImageFormatRgb, Vec3U16, Malachite::ImageFormatArgbFast, Vec4F>(Vec3U16 &dst, const Vec4F &src)
{
	Vec4F v = src * 0xFFFF;
	Vec4I32 iv = vecRound(v);
	
	dst.r = iv.r;
	dst.g = iv.g;
	dst.b = iv.b;
}

// 32bit float argb premultiplied -> 8bit unsigned argb
template < >
inline void convertPixel<Malachite::ImageFormatArgb, Vec4U8, Malachite::ImageFormatArgbFast, Vec4F>(Vec4U8 &dst, const Vec4F &src)
{
	Vec4F v = src;
	v.r *= v.a;
	v.g *= v.a;
	v.b *= v.a;
	
	v *= 0xFF;
	
	Vec4I32 iv = vecRound(v);
	
	dst.a = iv.a;
	dst.r = iv.r;
	dst.g = iv.g;
	dst.b = iv.b;
}

// 32bit float argb premultiplied -> 16bit unsigned argb
template < >
inline void convertPixel<Malachite::ImageFormatArgb, Vec4U16, Malachite::ImageFormatArgbFast, Vec4F>(Vec4U16 &dst, const Vec4F &src)
{
	Vec4F v = src;
	v.r *= v.a;
	v.g *= v.a;
	v.b *= v.a;
	
	v *= 0xFFFF;
	
	Vec4I32 iv = vecRound(v);
	
	dst.a = iv.a;
	dst.r = iv.r;
	dst.g = iv.g;
	dst.b = iv.b;
}

// 32bit float argb premultiplied -> 8bit unsigned argb premultiplied
template < >
inline void convertPixel<Malachite::ImageFormatArgbFast, Vec4U8, Malachite::ImageFormatArgbFast, Vec4F>(Vec4U8 &dst, const Vec4F &src)
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
inline void convertPixel<Malachite::ImageFormatArgbFast, Vec4U16, Malachite::ImageFormatArgbFast, Vec4F>(Vec4U16 &dst, const Vec4F &src)
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
