#ifndef MLBLENDOP_H
#define MLBLENDOP_H

//ExportName: BlendOp

#include "color.h"
#include "memory.h"

namespace Malachite
{

class MALACHITESHARED_EXPORT BlendOp
{
public:
	
	enum Tile
	{
		NoTile = 0b00,
		TileDestination = 0b01,
		TileSource = 0b10,
		TileBoth = 0b11
	};
	
	Q_DECLARE_FLAGS(TileCombination, Tile)
	
	virtual void blend(int count, Pointer<Pixel> dst, Pointer<const Pixel> src) = 0;
	virtual void blend(int count, Pointer<Pixel> dst, Pointer<const Pixel> src, Pointer<const Pixel> masks) = 0;
	virtual void blend(int count, Pointer<Pixel> dst, Pointer<const Pixel> src, Pointer<const float> opacities) = 0;
	virtual void blend(int count, Pointer<Pixel> dst, Pointer<const Pixel> src, const Pixel &mask) = 0;
	virtual void blend(int count, Pointer<Pixel> dst, Pointer<const Pixel> src, float opacity) = 0;
	virtual void blend(int count, Pointer<Pixel> dst, const Pixel &src) = 0;
	virtual void blend(int count, Pointer<Pixel> dst, const Pixel &src, Pointer<const Pixel> masks) = 0;
	virtual void blend(int count, Pointer<Pixel> dst, const Pixel &src, Pointer<const float> opacities) = 0;
	
	virtual void blendReversed(int count, Pointer<Pixel> dst, Pointer<const Pixel> src) = 0;
	virtual void blendReversed(int count, Pointer<Pixel> dst, Pointer<const Pixel> src, Pointer<const Pixel> masks) = 0;
	virtual void blendReversed(int count, Pointer<Pixel> dst, Pointer<const Pixel> src, Pointer<const float> opacities) = 0;
	virtual void blendReversed(int count, Pointer<Pixel> dst, Pointer<const Pixel> src, const Pixel &mask) = 0;
	virtual void blendReversed(int count, Pointer<Pixel> dst, Pointer<const Pixel> src, float opacity) = 0;
	
	virtual TileCombination tileRequirement(TileCombination combination) = 0;
};

/*
  
  example
  
class BlendFunctions
{
public:
	static void blendFunc(MLPointer<MLVec4F> dst, MLPointer<const MLVec4F> src)
	{ some code }
	
	static MLBlendOp::TileStates tileOpFunc(MLBlendOp::TileStates states)
	{ some code }
};

*/

/*
template <typename TBlendTraits>
class TemplateBlendOp : public BlendOp
{
public:
	void blend(int count, Pointer<Pixel> dst, Pointer<const Pixel> src)
	{
		while (count >= 4)
		{
			PixelVec dv0 = _mm_unpackhi_ps(dst[0], dst[1]);	// a1 a0 r1 r0
			PixelVec dv1 = _mm_unpackhi_ps(dst[2], dst[3]);	// a3 a2 r3 r2
			PixelVec dv2 = _mm_unpacklo_ps(dst[0], dst[1]); // g1 g0 b1 b0
			PixelVec dv3 = _mm_unpacklo_ps(dst[2], dst[3]); // g3 g2 b3 b2
			
			PixelVec da = _mm_movehl_ps(dv1, dv0);
			PixelVec dr = _mm_movelh_ps(dv0, dv1);
			PixelVec dg = _mm_movehl_ps(dv3, dv2);
			PixelVec db = _mm_movelh_ps(dv2, dv3);
			
			PixelVec ra = TBlendTraits::blendAlpha(da, sa);
			PixelVec rr = TBlendTraits::blendRed(da, dr, sa, sr);
			PixelVec rg = TBlendTraits::blendGreen(da, dg, sa, sg);
			PixelVec rb = TBlendTraits::blendBlue(da, db, sa, sb);
			
			PixelVec rv0 = _mm_unpacklo_ps(rr, ra);	// a1 r1 a0 r0
			PixelVec rv1 = _mm_unpackhi_ps(rr, ra); // a3 r3 a2 r2
			PixelVec rv2 = _mm_unpacklo_ps(rb, rg); // g1 b1 g0 b0
			PixelVec rv3 = _mm_unpackhi_ps(rb, rg); // g3 b3 g2 b2
			
			dst[0] = _mm_movelh_ps(rv2, rv0);
			dst[1] = _mm_movehl_ps(rv0, rv2);
			dst[2] = _mm_movelh_ps(rv3, rv1);
			dst[3] = _mm_movehl_ps(rv1, rv3);
		}
		
		for (int i = 0; i < count; ++i)
			BlendFunctions::blend(*dst++, *src++);
	}
	
	void blend(int count, Pointer<Pixel> dst, Pointer<const Pixel> src, Pointer<const Pixel> masks)
	{
		for (int i = 0; i < count; ++i)
			BlendFunctions::blend(*dst++, *src++ * (masks++)->a());
	}

	void blend(int count, Pointer<Pixel> dst, Pointer<const Pixel> src, Pointer<const float> opacities)
	{
		for (int i = 0; i < count; ++i)
			BlendFunctions::blend(*dst++, *src++ * *opacities++);
	}

	void blend(int count, Pointer<Pixel> dst, Pointer<const Pixel> src, const Pixel &mask)
	{
		for (int i = 0; i < count; ++i)
			BlendFunctions::blend(*dst++, *src++ * mask.a());
	}

	void blend(int count, Pointer<Pixel> dst, Pointer<const Pixel> src, float opacity)
	{
		for (int i = 0; i < count; ++i)
			BlendFunctions::blend(*dst++, *src++ * opacity);
	}
	
	void blend(int count, Pointer<Pixel> dst, const Pixel &src)
	{
		for (int i = 0; i < count; ++i)
			BlendFunctions::blend(*dst++, src);
	}
	
	void blend(int count, Pointer<Pixel> dst, const Pixel &src, Pointer<const Pixel> masks)
	{
		for (int i = 0; i < count; ++i)
			BlendFunctions::blend(*dst++, src * (masks++)->a());
	}
	
	void blend(int count, Pointer<Pixel> dst, const Pixel &src, Pointer<const float> opacities)
	{
		for (int i = 0; i < count; ++i)
			BlendFunctions::blend(*dst++, src * *opacities++);
	} 
	
	void blend(int count, Pointer<Pixel> dst, const Pixel &src, const Pixel &mask)
	{
		Pixel c = src * mask.a();
		for (int i = 0; i < count; ++i)
			BlendFunctions::blend(*dst++, c);
	}
	
	void blend(int count, Pointer<Pixel> dst, const Pixel &src, float opacity)
	{
		Pixel c = src * opacity;
		for (int i = 0; i < count; ++i)
			BlendFunctions::blend(*dst++, c);
	}
	
	
	
	void blendReversed(int count, Pointer<Pixel> dst, Pointer<const Pixel> src)
	{
		src += count - 1;
		
		for (int i = 0; i < count; ++i)
			BlendFunctions::blend(*dst++, *src--);
	}
	
	void blendReversed(int count, Pointer<Pixel> dst, Pointer<const Pixel> src, Pointer<const Pixel> masks)
	{
		src += count - 1;
		
		for (int i = 0; i < count; ++i)
			BlendFunctions::blend(*dst++, *src-- * (masks++)->a());
	}

	void blendReversed(int count, Pointer<Pixel> dst, Pointer<const Pixel> src, Pointer<const float> opacities)
	{
		src += count - 1;
		
		for (int i = 0; i < count; ++i)
			BlendFunctions::blend(*dst++, *src-- * *opacities++);
	}

	void blendReversed(int count, Pointer<Pixel> dst, Pointer<const Pixel> src, const Pixel &mask)
	{
		src += count - 1;
		
		for (int i = 0; i < count; ++i)
			BlendFunctions::blend(*dst++, *src-- * mask.a());
	}

	void blendReversed(int count, Pointer<Pixel> dst, Pointer<const Pixel> src, float opacity)
	{
		src += count - 1;
		
		for (int i = 0; i < count; ++i)
			BlendFunctions::blend(*dst++, *src-- * opacity);
	}
	
	TileCombination tileRequirement(TileCombination combination)
	{
		return BlendFunctions::tileRequirement(combination);
	}
};
*/

class MALACHITESHARED_EXPORT BlendOpDictionary
{
public:
	BlendOpDictionary();
	
	BlendOp *blendOp(int index) { return _blendOps.value(index, _defaultBlendOp); }
	
private:
	QHash<int, BlendOp *> _blendOps;
	BlendOp *_defaultBlendOp = 0;
};

MALACHITESHARED_EXPORT BlendOpDictionary *blendOpDictionary();

}

#endif // MLBLENDOP_H
