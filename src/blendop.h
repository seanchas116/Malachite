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
	virtual void blend(int count, Pointer<Pixel> dst, const Pixel &src, const Pixel &mask) = 0;
	virtual void blend(int count, Pointer<Pixel> dst, const Pixel &src, float opacity) = 0;
	
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

template <typename BlendFunctions>
class TemplateBlendOp : public BlendOp
{
public:
	void blend(int count, Pointer<Pixel> dst, Pointer<const Pixel> src)
	{
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

class MALACHITESHARED_EXPORT BlendOpDictionary
{
public:
	BlendOpDictionary();
	
	BlendOp *blendOp(int index) { return _blendOps.value(index); }
	
private:
	QHash<int, BlendOp *> _blendOps;
};

MALACHITESHARED_EXPORT BlendOpDictionary *blendOpDictionary();

}

#endif // MLBLENDOP_H
