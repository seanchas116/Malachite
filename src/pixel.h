#ifndef PIXEL_H
#define PIXEL_H

//ExportName: Pixel

#include <QtGlobal>
#include "global.h"
#include "genericpixel.h"

namespace Malachite
{

class MALACHITESHARED_EXPORT Pixel : public BgraPremultF
{
public:
	
	Pixel() : BgraPremultF() {}
	Pixel(float x) : BgraPremultF(x) {}
	Pixel(float a, float r, float g, float b) : BgraPremultF(a, r, g, b) {}
	Pixel(const VectorType &v) : BgraPremultF(v) {}
	
	template <PixelParams::Premult Other_Premult, PixelParams::Alpha Other_Alpha, typename Other_Index, typename Other_Channel>
	Pixel(const RgbPixel<Other_Premult, Other_Alpha, Other_Index, Other_Channel> &other) : BgraPremultF(other) {}
	
	VectorType aV() const { return _v.extract(Index::A); }
	
	Pixel operator*(float f) const
	{
		Pixel p;
		p.rv() = v() * VectorType(f);
		return p;
	}
	
	Pixel &operator*=(float f)
	{
		rv() *= VectorType(f);
		return *this;
	}
	
	operator VectorType() const { return _v; }
};

typedef Pixel::VectorType PixelVec;

}

Q_DECLARE_METATYPE(Malachite::Pixel)

#endif // PIXEL_H
