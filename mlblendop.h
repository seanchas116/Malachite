#ifndef MLBLENDOP_H
#define MLBLENDOP_H

#include "mlcolor.h"
#include "mlmemory.h"


class MALACHITESHARED_EXPORT MLBlendOp
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
	
	virtual void blend(int count, MLPointer<MLVec4F> dst, MLPointer<const MLVec4F> src) = 0;
	virtual void blend(int count, MLPointer<MLVec4F> dst, MLPointer<const MLVec4F> src, MLPointer<const MLVec4F> masks) = 0;
	virtual void blend(int count, MLPointer<MLVec4F> dst, MLPointer<const MLVec4F> src, MLPointer<const float> opacities) = 0;
	virtual void blend(int count, MLPointer<MLVec4F> dst, MLPointer<const MLVec4F> src, const MLVec4F &mask) = 0;
	virtual void blend(int count, MLPointer<MLVec4F> dst, MLPointer<const MLVec4F> src, float opacity) = 0;
	virtual void blend(int count, MLPointer<MLVec4F> dst, const MLVec4F &src) = 0;
	virtual void blend(int count, MLPointer<MLVec4F> dst, const MLVec4F &src, MLPointer<const MLVec4F> masks) = 0;
	virtual void blend(int count, MLPointer<MLVec4F> dst, const MLVec4F &src, MLPointer<const float> opacities) = 0;
	virtual void blend(int count, MLPointer<MLVec4F> dst, const MLVec4F &src, const MLVec4F &mask) = 0;
	virtual void blend(int count, MLPointer<MLVec4F> dst, const MLVec4F &src, float opacity) = 0;
	
	virtual void blendReversed(int count, MLPointer<MLVec4F> dst, MLPointer<const MLVec4F> src) = 0;
	virtual void blendReversed(int count, MLPointer<MLVec4F> dst, MLPointer<const MLVec4F> src, MLPointer<const MLVec4F> masks) = 0;
	virtual void blendReversed(int count, MLPointer<MLVec4F> dst, MLPointer<const MLVec4F> src, MLPointer<const float> opacities) = 0;
	virtual void blendReversed(int count, MLPointer<MLVec4F> dst, MLPointer<const MLVec4F> src, const MLVec4F &mask) = 0;
	virtual void blendReversed(int count, MLPointer<MLVec4F> dst, MLPointer<const MLVec4F> src, float opacity) = 0;
	
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
class MLTemplateBlendOp : public MLBlendOp
{
public:
	void blend(int count, MLPointer<MLVec4F> dst, MLPointer<const MLVec4F> src)
	{
		for (int i = 0; i < count; ++i)
			BlendFunctions::blend(*dst++, *src++);
	}
	
	void blend(int count, MLPointer<MLVec4F> dst, MLPointer<const MLVec4F> src, MLPointer<const MLVec4F> masks)
	{
		for (int i = 0; i < count; ++i)
			BlendFunctions::blend(*dst++, *src++ * (masks++)->a);
	}

	void blend(int count, MLPointer<MLVec4F> dst, MLPointer<const MLVec4F> src, MLPointer<const float> opacities)
	{
		for (int i = 0; i < count; ++i)
			BlendFunctions::blend(*dst++, *src++ * *opacities++);
	}

	void blend(int count, MLPointer<MLVec4F> dst, MLPointer<const MLVec4F> src, const MLVec4F &mask)
	{
		for (int i = 0; i < count; ++i)
			BlendFunctions::blend(*dst++, *src++ * mask.a);
	}

	void blend(int count, MLPointer<MLVec4F> dst, MLPointer<const MLVec4F> src, float opacity)
	{
		for (int i = 0; i < count; ++i)
			BlendFunctions::blend(*dst++, *src++ * opacity);
	}
	
	void blend(int count, MLPointer<MLVec4F> dst, const MLVec4F &src)
	{
		for (int i = 0; i < count; ++i)
			BlendFunctions::blend(*dst++, src);
	}
	
	void blend(int count, MLPointer<MLVec4F> dst, const MLVec4F &src, MLPointer<const MLVec4F> masks)
	{
		for (int i = 0; i < count; ++i)
			BlendFunctions::blend(*dst++, src * (masks++)->a);
	}
	
	void blend(int count, MLPointer<MLVec4F> dst, const MLVec4F &src, MLPointer<const float> opacities)
	{
		for (int i = 0; i < count; ++i)
			BlendFunctions::blend(*dst++, src * *opacities++);
	} 
	
	void blend(int count, MLPointer<MLVec4F> dst, const MLVec4F &src, const MLVec4F &mask)
	{
		MLVec4F c = src * mask.a;
		for (int i = 0; i < count; ++i)
			BlendFunctions::blend(*dst++, c);
	}
	
	void blend(int count, MLPointer<MLVec4F> dst, const MLVec4F &src, float opacity)
	{
		MLVec4F c = src * opacity;
		for (int i = 0; i < count; ++i)
			BlendFunctions::blend(*dst++, c);
	}
	
	
	
	void blendReversed(int count, MLPointer<MLVec4F> dst, MLPointer<const MLVec4F> src)
	{
		src += count - 1;
		
		for (int i = 0; i < count; ++i)
			BlendFunctions::blend(*dst++, *src--);
	}
	
	void blendReversed(int count, MLPointer<MLVec4F> dst, MLPointer<const MLVec4F> src, MLPointer<const MLVec4F> masks)
	{
		src += count - 1;
		
		for (int i = 0; i < count; ++i)
			BlendFunctions::blend(*dst++, *src-- * (masks++)->a);
	}

	void blendReversed(int count, MLPointer<MLVec4F> dst, MLPointer<const MLVec4F> src, MLPointer<const float> opacities)
	{
		src += count - 1;
		
		for (int i = 0; i < count; ++i)
			BlendFunctions::blend(*dst++, *src-- * *opacities++);
	}

	void blendReversed(int count, MLPointer<MLVec4F> dst, MLPointer<const MLVec4F> src, const MLVec4F &mask)
	{
		src += count - 1;
		
		for (int i = 0; i < count; ++i)
			BlendFunctions::blend(*dst++, *src-- * mask.a);
	}

	void blendReversed(int count, MLPointer<MLVec4F> dst, MLPointer<const MLVec4F> src, float opacity)
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

class MLBlendOpDictionary
{
public:
	MLBlendOpDictionary();
	
	MLBlendOp *blendOp(int index) { return _blendOps.value(index); }
	
private:
	QHash<int, MLBlendOp *> _blendOps;
};

MLBlendOpDictionary *mlBlendOpDictionary();

#endif // MLBLENDOP_H
