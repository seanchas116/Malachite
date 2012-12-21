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
	
	virtual void blend(int count, Pointer<Vec4F> dst, Pointer<const Vec4F> src) = 0;
	virtual void blend(int count, Pointer<Vec4F> dst, Pointer<const Vec4F> src, Pointer<const Vec4F> masks) = 0;
	virtual void blend(int count, Pointer<Vec4F> dst, Pointer<const Vec4F> src, Pointer<const float> opacities) = 0;
	virtual void blend(int count, Pointer<Vec4F> dst, Pointer<const Vec4F> src, const Vec4F &mask) = 0;
	virtual void blend(int count, Pointer<Vec4F> dst, Pointer<const Vec4F> src, float opacity) = 0;
	virtual void blend(int count, Pointer<Vec4F> dst, const Vec4F &src) = 0;
	virtual void blend(int count, Pointer<Vec4F> dst, const Vec4F &src, Pointer<const Vec4F> masks) = 0;
	virtual void blend(int count, Pointer<Vec4F> dst, const Vec4F &src, Pointer<const float> opacities) = 0;
	virtual void blend(int count, Pointer<Vec4F> dst, const Vec4F &src, const Vec4F &mask) = 0;
	virtual void blend(int count, Pointer<Vec4F> dst, const Vec4F &src, float opacity) = 0;
	
	virtual void blendReversed(int count, Pointer<Vec4F> dst, Pointer<const Vec4F> src) = 0;
	virtual void blendReversed(int count, Pointer<Vec4F> dst, Pointer<const Vec4F> src, Pointer<const Vec4F> masks) = 0;
	virtual void blendReversed(int count, Pointer<Vec4F> dst, Pointer<const Vec4F> src, Pointer<const float> opacities) = 0;
	virtual void blendReversed(int count, Pointer<Vec4F> dst, Pointer<const Vec4F> src, const Vec4F &mask) = 0;
	virtual void blendReversed(int count, Pointer<Vec4F> dst, Pointer<const Vec4F> src, float opacity) = 0;
	
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
	void blend(int count, Pointer<Vec4F> dst, Pointer<const Vec4F> src)
	{
		for (int i = 0; i < count; ++i)
			BlendFunctions::blend(*dst++, *src++);
	}
	
	void blend(int count, Pointer<Vec4F> dst, Pointer<const Vec4F> src, Pointer<const Vec4F> masks)
	{
		for (int i = 0; i < count; ++i)
			BlendFunctions::blend(*dst++, *src++ * (masks++)->a);
	}

	void blend(int count, Pointer<Vec4F> dst, Pointer<const Vec4F> src, Pointer<const float> opacities)
	{
		for (int i = 0; i < count; ++i)
			BlendFunctions::blend(*dst++, *src++ * *opacities++);
	}

	void blend(int count, Pointer<Vec4F> dst, Pointer<const Vec4F> src, const Vec4F &mask)
	{
		for (int i = 0; i < count; ++i)
			BlendFunctions::blend(*dst++, *src++ * mask.a);
	}

	void blend(int count, Pointer<Vec4F> dst, Pointer<const Vec4F> src, float opacity)
	{
		for (int i = 0; i < count; ++i)
			BlendFunctions::blend(*dst++, *src++ * opacity);
	}
	
	void blend(int count, Pointer<Vec4F> dst, const Vec4F &src)
	{
		for (int i = 0; i < count; ++i)
			BlendFunctions::blend(*dst++, src);
	}
	
	void blend(int count, Pointer<Vec4F> dst, const Vec4F &src, Pointer<const Vec4F> masks)
	{
		for (int i = 0; i < count; ++i)
			BlendFunctions::blend(*dst++, src * (masks++)->a);
	}
	
	void blend(int count, Pointer<Vec4F> dst, const Vec4F &src, Pointer<const float> opacities)
	{
		for (int i = 0; i < count; ++i)
			BlendFunctions::blend(*dst++, src * *opacities++);
	} 
	
	void blend(int count, Pointer<Vec4F> dst, const Vec4F &src, const Vec4F &mask)
	{
		Vec4F c = src * mask.a;
		for (int i = 0; i < count; ++i)
			BlendFunctions::blend(*dst++, c);
	}
	
	void blend(int count, Pointer<Vec4F> dst, const Vec4F &src, float opacity)
	{
		Vec4F c = src * opacity;
		for (int i = 0; i < count; ++i)
			BlendFunctions::blend(*dst++, c);
	}
	
	
	
	void blendReversed(int count, Pointer<Vec4F> dst, Pointer<const Vec4F> src)
	{
		src += count - 1;
		
		for (int i = 0; i < count; ++i)
			BlendFunctions::blend(*dst++, *src--);
	}
	
	void blendReversed(int count, Pointer<Vec4F> dst, Pointer<const Vec4F> src, Pointer<const Vec4F> masks)
	{
		src += count - 1;
		
		for (int i = 0; i < count; ++i)
			BlendFunctions::blend(*dst++, *src-- * (masks++)->a);
	}

	void blendReversed(int count, Pointer<Vec4F> dst, Pointer<const Vec4F> src, Pointer<const float> opacities)
	{
		src += count - 1;
		
		for (int i = 0; i < count; ++i)
			BlendFunctions::blend(*dst++, *src-- * *opacities++);
	}

	void blendReversed(int count, Pointer<Vec4F> dst, Pointer<const Vec4F> src, const Vec4F &mask)
	{
		src += count - 1;
		
		for (int i = 0; i < count; ++i)
			BlendFunctions::blend(*dst++, *src-- * mask.a);
	}

	void blendReversed(int count, Pointer<Vec4F> dst, Pointer<const Vec4F> src, float opacity)
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

class BlendOpDictionary
{
public:
	BlendOpDictionary();
	
	BlendOp *blendOp(int index) { return _blendOps.value(index); }
	
private:
	QHash<int, BlendOp *> _blendOps;
};

BlendOpDictionary *blendOpDictionary();

}

#endif // MLBLENDOP_H
