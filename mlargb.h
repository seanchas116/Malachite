#ifndef MLARGB_H
#define MLARGB_H

#include <stdint.h>
#include "mlsimdvector.h"

struct MLArgb8;
struct MLRgba8;
struct MLArgb16;
struct MLRgba16;
struct MLArgbF;

struct MLFastArgb8;
struct MLFastArgbF;

struct MLRgb8;
struct MLRgb16;


struct MLArgb8
{
	uint8_t &a() { return v[MLGlobal::ArgbAlpha]; }
	uint8_t &r() { return v[MLGlobal::ArgbRed]; }
	uint8_t &g() { return v[MLGlobal::ArgbGreen]; }
	uint8_t &b() { return v[MLGlobal::ArgbBlue]; }
	
	const uint8_t &a() const { return v[MLGlobal::ArgbAlpha]; }
	const uint8_t &r() const { return v[MLGlobal::ArgbRed]; }
	const uint8_t &g() const { return v[MLGlobal::ArgbGreen]; }
	const uint8_t &b() const { return v[MLGlobal::ArgbBlue]; }
	
	MLArgb8 &operator=(const MLFastArgbF &fArgbF);
	
	uint8_t v[4];
};

struct MLRgba8
{
	uint8_t &r() { return v[MLGlobal::RgbaRed]; }
	uint8_t &g() { return v[MLGlobal::RgbaGreen]; }
	uint8_t &b() { return v[MLGlobal::RgbaBlue]; }
	uint8_t &a() { return v[MLGlobal::RgbaAlpha]; }
	
	const uint8_t &r() const { return v[MLGlobal::RgbaRed]; }
	const uint8_t &g() const { return v[MLGlobal::RgbaGreen]; }
	const uint8_t &b() const { return v[MLGlobal::RgbaBlue]; }
	const uint8_t &a() const { return v[MLGlobal::RgbaAlpha]; }
	
	MLRgba8 &operator=(const MLFastArgbF &fArgbF);
	
	uint8_t v[4];
};

struct MLArgb16
{
	uint16_t &a() { return v[MLGlobal::ArgbAlpha]; }
	uint16_t &r() { return v[MLGlobal::ArgbRed]; }
	uint16_t &g() { return v[MLGlobal::ArgbGreen]; }
	uint16_t &b() { return v[MLGlobal::ArgbBlue]; }
	
	const uint16_t &a() const { return v[MLGlobal::ArgbAlpha]; }
	const uint16_t &r() const { return v[MLGlobal::ArgbRed]; }
	const uint16_t &g() const { return v[MLGlobal::ArgbGreen]; }
	const uint16_t &b() const { return v[MLGlobal::ArgbBlue]; }
	
	MLArgb16 &operator=(const MLFastArgbF &fArgbF);
	
	uint16_t v[4];
};

struct MLRgba16
{
	uint16_t &r() { return v[MLGlobal::RgbaRed]; }
	uint16_t &g() { return v[MLGlobal::RgbaGreen]; }
	uint16_t &b() { return v[MLGlobal::RgbaBlue]; }
	uint16_t &a() { return v[MLGlobal::RgbaAlpha]; }
	
	const uint16_t &r() const { return v[MLGlobal::RgbaRed]; }
	const uint16_t &g() const { return v[MLGlobal::RgbaGreen]; }
	const uint16_t &b() const { return v[MLGlobal::RgbaBlue]; }
	const uint16_t &a() const { return v[MLGlobal::RgbaAlpha]; }
	
	MLRgba16 &operator=(const MLFastArgbF &fArgbF);
	
	uint16_t v[4];
};

struct MLArgbF
{
	float &a() { return v[MLGlobal::ArgbAlpha]; }
	float &r() { return v[MLGlobal::ArgbRed]; }
	float &g() { return v[MLGlobal::ArgbGreen]; }
	float &b() { return v[MLGlobal::ArgbBlue]; }
	
	const float &a() const { return v[MLGlobal::ArgbAlpha]; }
	const float &r() const { return v[MLGlobal::ArgbRed]; }
	const float &g() const { return v[MLGlobal::ArgbGreen]; }
	const float &b() const { return v[MLGlobal::ArgbBlue]; }
	
	MLSimdF4 v;
};

struct MLFastArgb8
{
	uint8_t &a() { return v[MLGlobal::ArgbAlpha]; }
	uint8_t &r() { return v[MLGlobal::ArgbRed]; }
	uint8_t &g() { return v[MLGlobal::ArgbGreen]; }
	uint8_t &b() { return v[MLGlobal::ArgbBlue]; }
	
	const uint8_t &a() const { return v[MLGlobal::ArgbAlpha]; }
	const uint8_t &r() const { return v[MLGlobal::ArgbRed]; }
	const uint8_t &g() const { return v[MLGlobal::ArgbGreen]; }
	const uint8_t &b() const { return v[MLGlobal::ArgbBlue]; }
	
	MLFastArgb8 &operator=(const MLFastArgbF &fArgbF);
	
	uint8_t v[4];
};

struct MLFastArgbF
{
	float &a() { return v[MLGlobal::ArgbAlpha]; }
	float &r() { return v[MLGlobal::ArgbRed]; }
	float &g() { return v[MLGlobal::ArgbGreen]; }
	float &b() { return v[MLGlobal::ArgbBlue]; }
	
	const float &a() const { return v[MLGlobal::ArgbAlpha]; }
	const float &r() const { return v[MLGlobal::ArgbRed]; }
	const float &g() const { return v[MLGlobal::ArgbGreen]; }
	const float &b() const { return v[MLGlobal::ArgbBlue]; }
	
	MLFastArgbF &operator=(const MLArgb8 &argb8);
	MLFastArgbF &operator=(const MLRgba8 &rgba8);
	MLFastArgbF &operator=(const MLArgb16 &argb16);
	MLFastArgbF &operator=(const MLRgba16 &rgba16);
	
	MLFastArgbF &operator=(const MLFastArgb8 &argb);
	
	MLFastArgbF &operator=(const MLRgb8 &rgb8);
	MLFastArgbF &operator=(const MLRgb16 &rgb16);
	
	void clamp() { v = mlSimdBound(0.f, v, 1.f); }
	
	MLSimdF4 v;
};

struct MLRgb8
{
	uint8_t &r() { return v[MLGlobal::RgbRed]; }
	uint8_t &g() { return v[MLGlobal::RgbGreen]; }
	uint8_t &b() { return v[MLGlobal::RgbBlue]; }
	
	const uint8_t &r() const { return v[MLGlobal::RgbRed]; }
	const uint8_t &g() const { return v[MLGlobal::RgbGreen]; }
	const uint8_t &b() const { return v[MLGlobal::RgbBlue]; }
	
	MLRgb8 &operator=(const MLFastArgbF &fArgbF);
	
	uint8_t v[3];
};

struct MLRgb8ArgbWrapper
{
	MLRgb8ArgbWrapper(const MLRgb8 &rgb) : rgb(rgb) {}
	
	uint8_t a() const { return 0xFF; }
	uint8_t &r() { return rgb.r(); }
	uint8_t &g() { return rgb.g(); }
	uint8_t &b() { return rgb.b(); }
	
	const uint8_t &r() const { return rgb.r(); }
	const uint8_t &g() const { return rgb.g(); }
	const uint8_t &b() const { return rgb.b(); }
	
	MLRgb8 rgb;
};

struct MLRgb16
{
	uint16_t &r() { return v[MLGlobal::RgbRed]; }
	uint16_t &g() { return v[MLGlobal::RgbGreen]; }
	uint16_t &b() { return v[MLGlobal::RgbBlue]; }
	
	const uint16_t &r() const { return v[MLGlobal::RgbRed]; }
	const uint16_t &g() const { return v[MLGlobal::RgbGreen]; }
	const uint16_t &b() const { return v[MLGlobal::RgbBlue]; }
	
	MLRgb16 &operator=(const MLFastArgbF &fArgbF);
	
	uint16_t v[3];
};

struct MLRgb16ArgbWrapper
{
	MLRgb16ArgbWrapper(const MLRgb16 &rgb) : rgb(rgb) {}
	
	uint16_t a() const { return 0xFFFF; }
	uint16_t &r() { return rgb.r(); }
	uint16_t &g() { return rgb.g(); }
	uint16_t &b() { return rgb.b(); }
	
	const uint16_t &r() const { return rgb.r(); }
	const uint16_t &g() const { return rgb.g(); }
	const uint16_t &b() const { return rgb.b(); }
	
	MLRgb16 rgb;
};




template <int DstBitWidth, typename Dst, typename Src>
void mlArgbFToInt(Dst &dst, const Src &srcArgb)
{
	MLSimd32I4 iv;
	iv = mlSimdRound(srcArgb.v * ((1 << DstBitWidth) - 1));
	dst.a() = iv[MLGlobal::ArgbAlpha];
	dst.r() = iv[MLGlobal::ArgbRed];
	dst.g() = iv[MLGlobal::ArgbGreen];
	dst.b() = iv[MLGlobal::ArgbBlue];
}

template <int SrcBitWidth, typename Dst, typename Src>
void mlArgbFFromInt(Dst &dstArgb, const Src &src)
{
	MLSimd32I4 iv;
	iv[MLGlobal::ArgbAlpha] = src.a();
	iv[MLGlobal::ArgbRed] = src.r();
	iv[MLGlobal::ArgbGreen] = src.g();
	iv[MLGlobal::ArgbBlue] = src.b();
	dstArgb.v = iv;
	dstArgb.v /= ((1 << SrcBitWidth) - 1);
}

inline MLFastArgbF mlArgbFNormalToFast(const MLArgbF &argbf)
{
	MLFastArgbF fargbf;
	fargbf.v = argbf.v * argbf.a();
	fargbf.a() = argbf.a();
	return fargbf;
}

inline MLArgbF mlArgbFFastToNormal(const MLFastArgbF &fargbf)
{
	MLArgbF argbf;
	
	if (fargbf.a() == 0)
	{
		argbf.v = MLSimdF4(0);
	}
	else
	{
		argbf.v = fargbf.v / fargbf.a();
		argbf.a() = fargbf.a();
	}
	
	return argbf;
}




inline MLArgb8 &MLArgb8::operator =(const MLFastArgbF &fArgbF)
{
	mlArgbFToInt<8>(*this, mlArgbFFastToNormal(fArgbF));
	return *this;
}

inline MLRgba8 &MLRgba8::operator =(const MLFastArgbF &fArgbF)
{
	mlArgbFToInt<8>(*this, mlArgbFFastToNormal(fArgbF));
	return *this;
}

inline MLArgb16 &MLArgb16::operator =(const MLFastArgbF &fArgbF)
{
	mlArgbFToInt<16>(*this, mlArgbFFastToNormal(fArgbF));
	return *this;
}

inline MLRgba16 &MLRgba16::operator =(const MLFastArgbF &fArgbF)
{
	mlArgbFToInt<16>(*this, mlArgbFFastToNormal(fArgbF));
	return *this;
}

inline MLFastArgb8 &MLFastArgb8::operator =(const MLFastArgbF &fargbf)
{
	mlArgbFToInt<8>(*this, fargbf);
	return *this;
}

inline MLFastArgbF &MLFastArgbF::operator =(const MLArgb8 &argb)
{
	MLArgbF argbF;
	mlArgbFFromInt<8>(argbF, argb);
	return *this = mlArgbFNormalToFast(argbF);
}

inline MLFastArgbF &MLFastArgbF::operator =(const MLRgba8 &argb)
{
	MLArgbF argbF;
	mlArgbFFromInt<8>(argbF, argb);
	return *this = mlArgbFNormalToFast(argbF);
}

inline MLFastArgbF &MLFastArgbF::operator =(const MLArgb16 &argb)
{
	MLArgbF argbF;
	mlArgbFFromInt<16>(argbF, argb);
	return *this = mlArgbFNormalToFast(argbF);
}

inline MLFastArgbF &MLFastArgbF::operator =(const MLRgba16 &argb)
{
	MLArgbF argbF;
	mlArgbFFromInt<16>(argbF, argb);
	return *this = mlArgbFNormalToFast(argbF);
}

inline MLFastArgbF &MLFastArgbF::operator =(const MLFastArgb8 &argb)
{
	mlArgbFFromInt<8>(*this, argb);
	return *this;
}

inline MLFastArgbF &MLFastArgbF::operator =(const MLRgb8 &rgb)
{
	mlArgbFFromInt<8>(*this, MLRgb8ArgbWrapper(rgb));
	return *this;
}

inline MLFastArgbF &MLFastArgbF::operator =(const MLRgb16 &rgb)
{
	mlArgbFFromInt<16>(*this, MLRgb16ArgbWrapper(rgb));
	return *this;
}

inline MLRgb8 &MLRgb8::operator =(const MLFastArgbF &fArgbF)
{
	MLSimdF4 fv = fArgbF.v + (1.0f - fArgbF.a());
	MLSimd32I4 iv;
	iv = mlSimdRound(fv * 0xFF);
	
	r() = iv[MLGlobal::ArgbRed];
	g() = iv[MLGlobal::ArgbGreen];
	b() = iv[MLGlobal::ArgbBlue];
	
	return *this;
}

inline MLRgb16 &MLRgb16::operator =(const MLFastArgbF &fArgbF)
{
	MLSimdF4 fv = fArgbF.v + (1.0f - fArgbF.a());
	MLSimd32I4 iv;
	iv = mlSimdRound(fv * 0xFFFF);
	
	r() = iv[MLGlobal::ArgbRed];
	g() = iv[MLGlobal::ArgbGreen];
	b() = iv[MLGlobal::ArgbBlue];
	
	return *this;
}


typedef MLFastArgbF MLArgb;
Q_DECLARE_METATYPE(MLArgb)

#endif // MLARGB_H
