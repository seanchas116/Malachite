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
	uint8_t &a() { return v[3]; }
	uint8_t &r() { return v[2]; }
	uint8_t &g() { return v[1]; }
	uint8_t &b() { return v[0]; }
	
	const uint8_t &a() const { return v[3]; }
	const uint8_t &r() const { return v[2]; }
	const uint8_t &g() const { return v[1]; }
	const uint8_t &b() const { return v[0]; }
	
	MLArgb8 &operator=(const MLFastArgbF &fArgbF);
	
	uint8_t v[4];
};

struct MLRgba8
{
	uint8_t &r() { return v[3]; }
	uint8_t &g() { return v[2]; }
	uint8_t &b() { return v[1]; }
	uint8_t &a() { return v[0]; }
	
	const uint8_t &r() const { return v[3]; }
	const uint8_t &g() const { return v[2]; }
	const uint8_t &b() const { return v[1]; }
	const uint8_t &a() const { return v[0]; }
	
	MLRgba8 &operator=(const MLFastArgbF &fArgbF);
	
	uint8_t v[4];
};

struct MLArgb16
{
	uint16_t &a() { return v[3]; }
	uint16_t &r() { return v[2]; }
	uint16_t &g() { return v[1]; }
	uint16_t &b() { return v[0]; }
	
	const uint16_t &a() const { return v[3]; }
	const uint16_t &r() const { return v[2]; }
	const uint16_t &g() const { return v[1]; }
	const uint16_t &b() const { return v[0]; }
	
	MLArgb16 &operator=(const MLFastArgbF &fArgbF);
	
	uint16_t v[4];
};

struct MLRgba16
{
	uint16_t &r() { return v[3]; }
	uint16_t &g() { return v[2]; }
	uint16_t &b() { return v[1]; }
	uint16_t &a() { return v[0]; }
	
	const uint16_t &r() const { return v[3]; }
	const uint16_t &g() const { return v[2]; }
	const uint16_t &b() const { return v[1]; }
	const uint16_t &a() const { return v[0]; }
	
	MLRgba16 &operator=(const MLFastArgbF &fArgbF);
	
	uint16_t v[4];
};

struct MLArgbF
{
	float &a() { return v[3]; }
	float &r() { return v[2]; }
	float &g() { return v[1]; }
	float &b() { return v[0]; }
	
	const float &a() const { return v[3]; }
	const float &r() const { return v[2]; }
	const float &g() const { return v[1]; }
	const float &b() const { return v[0]; }
	
	MLSimdF4 v;
};

struct MLFastArgb8
{
	uint8_t &a() { return v[3]; }
	uint8_t &r() { return v[2]; }
	uint8_t &g() { return v[1]; }
	uint8_t &b() { return v[0]; }
	
	const uint8_t &a() const { return v[3]; }
	const uint8_t &r() const { return v[2]; }
	const uint8_t &g() const { return v[1]; }
	const uint8_t &b() const { return v[0]; }
	
	MLFastArgb8 &operator=(const MLFastArgbF &fArgbF);
	
	uint8_t v[4];
};

struct MLFastArgbF
{
	float &a() { return v[3]; }
	float &r() { return v[2]; }
	float &g() { return v[1]; }
	float &b() { return v[0]; }
	
	const float &a() const { return v[3]; }
	const float &r() const { return v[2]; }
	const float &g() const { return v[1]; }
	const float &b() const { return v[0]; }
	
	MLFastArgbF &operator=(const MLArgb8 &argb8);
	MLFastArgbF &operator=(const MLRgba8 &rgba8);
	MLFastArgbF &operator=(const MLArgb16 &argb16);
	MLFastArgbF &operator=(const MLRgba16 &rgba16);
	
	MLFastArgbF &operator=(const MLFastArgb8 &argb);
	
	MLFastArgbF &operator=(const MLRgb8 &rgb8);
	MLFastArgbF &operator=(const MLRgb16 &rgb16);
	
	MLSimdF4 v;
};

struct MLRgb8
{
	uint8_t &r() { return v[2]; }
	uint8_t &g() { return v[1]; }
	uint8_t &b() { return v[0]; }
	
	const uint8_t &r() const { return v[2]; }
	const uint8_t &g() const { return v[1]; }
	const uint8_t &b() const { return v[0]; }
	
	MLRgb8 &operator=(const MLFastArgbF &fArgbF);
	
	uint8_t v[3];
};

struct MLRgb8ArgbWrapper
{
	MLRgb8ArgbWrapper(const MLRgb8 &rgb) : rgb(rgb) {}
	
	uint8_t a() const { return 0xFF; }
	uint8_t &r() { return rgb.v[2]; }
	uint8_t &g() { return rgb.v[1]; }
	uint8_t &b() { return rgb.v[0]; }
	
	const uint8_t &r() const { return rgb.v[2]; }
	const uint8_t &g() const { return rgb.v[1]; }
	const uint8_t &b() const { return rgb.v[0]; }
	
	MLRgb8 rgb;
};

struct MLRgb16
{
	uint16_t &r() { return v[2]; }
	uint16_t &g() { return v[1]; }
	uint16_t &b() { return v[0]; }
	
	const uint16_t &r() const { return v[2]; }
	const uint16_t &g() const { return v[1]; }
	const uint16_t &b() const { return v[0]; }
	
	MLRgb16 &operator=(const MLFastArgbF &fArgbF);
	
	uint16_t v[3];
};

struct MLRgb16ArgbWrapper
{
	MLRgb16ArgbWrapper(const MLRgb16 &rgb) : rgb(rgb) {}
	
	uint16_t a() const { return 0xFFFF; }
	uint16_t &r() { return rgb.v[2]; }
	uint16_t &g() { return rgb.v[1]; }
	uint16_t &b() { return rgb.v[0]; }
	
	const uint16_t &r() const { return rgb.v[2]; }
	const uint16_t &g() const { return rgb.v[1]; }
	const uint16_t &b() const { return rgb.v[0]; }
	
	MLRgb16 rgb;
};




template <int DstBitWidth, typename Dst, typename Src>
void mlArgbFToInt(Dst &dst, const Src &srcArgb)
{
	MLSimd32I4 iv;
	iv = mlSimdRound(srcArgb.v * ((1 << DstBitWidth) - 1));
	dst.a() = iv[3];
	dst.r() = iv[2];
	dst.g() = iv[1];
	dst.b() = iv[0];
}

template <int SrcBitWidth, typename Dst, typename Src>
void mlArgbFFromInt(Dst &dstArgb, const Src &src)
{
	MLSimd32I4 iv;
	iv[3] = src.a();
	iv[2] = src.r();
	iv[1] = src.g();
	iv[0] = src.b();
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
	
	r() = iv[2];
	g() = iv[1];
	b() = iv[0];
	
	return *this;
}

inline MLRgb16 &MLRgb16::operator =(const MLFastArgbF &fArgbF)
{
	MLSimdF4 fv = fArgbF.v + (1.0f - fArgbF.a());
	MLSimd32I4 iv;
	iv = mlSimdRound(fv * 0xFFFF);
	
	r() = iv[2];
	g() = iv[1];
	b() = iv[0];
	
	return *this;
}


typedef MLFastArgbF MLArgb;
Q_DECLARE_METATYPE(MLArgb)

#endif // MLARGB_H
