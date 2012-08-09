#ifndef MLBLENDOP_H
#define MLBLENDOP_H

#include "mlcolor.h"
#include "mlmemory.h"


class MALACHITESHARED_EXPORT MLBlendOp
{
public:
	
	enum TileState
	{
		NoTiles = 0x0,
		TileDestination = 0x1,
		TileSource = 0x2,
		TileBoth = 0x3
	};
	
	Q_DECLARE_FLAGS(TileStates, TileState)
	
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
	
	virtual TileStates tileOp(TileStates states) = 0;
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
			BlendFunctions::blendFunc(*dst++, *src++);
	}
	
	void blend(int count, MLPointer<MLVec4F> dst, MLPointer<const MLVec4F> src, MLPointer<const MLVec4F> masks)
	{
		for (int i = 0; i < count; ++i)
			BlendFunctions::blendFunc(*dst++, *src++ * (masks++)->a);
	}

	void blend(int count, MLPointer<MLVec4F> dst, MLPointer<const MLVec4F> src, MLPointer<const float> opacities)
	{
		for (int i = 0; i < count; ++i)
			BlendFunctions::blendFunc(*dst++, *src++ * *opacities++);
	}

	void blend(int count, MLPointer<MLVec4F> dst, MLPointer<const MLVec4F> src, const MLVec4F &mask)
	{
		for (int i = 0; i < count; ++i)
			BlendFunctions::blendFunc(*dst++, *src++ * mask.a);
	}

	void blend(int count, MLPointer<MLVec4F> dst, MLPointer<const MLVec4F> src, float opacity)
	{
		for (int i = 0; i < count; ++i)
			BlendFunctions::blendFunc(*dst++, *src++ * opacity);
	}
	
	void blend(int count, MLPointer<MLVec4F> dst, const MLVec4F &src)
	{
		for (int i = 0; i < count; ++i)
			BlendFunctions::blendFunc(*dst++, src);
	}
	
	void blend(int count, MLPointer<MLVec4F> dst, const MLVec4F &src, MLPointer<const MLVec4F> masks)
	{
		for (int i = 0; i < count; ++i)
			BlendFunctions::blendFunc(*dst++, src * (masks++)->a);
	}
	
	void blend(int count, MLPointer<MLVec4F> dst, const MLVec4F &src, MLPointer<const float> opacities)
	{
		for (int i = 0; i < count; ++i)
			BlendFunctions::blendFunc(*dst++, src * *opacities++);
	} 
	
	void blend(int count, MLPointer<MLVec4F> dst, const MLVec4F &src, const MLVec4F &mask)
	{
		MLVec4F c = src * mask.a;
		for (int i = 0; i < count; ++i)
			BlendFunctions::blendFunc(*dst++, c);
	}
	
	void blend(int count, MLPointer<MLVec4F> dst, const MLVec4F &src, float opacity)
	{
		MLVec4F c = src * opacity;
		for (int i = 0; i < count; ++i)
			BlendFunctions::blendFunc(*dst++, c);
	}
	
	
	
	void blendReversed(int count, MLPointer<MLVec4F> dst, MLPointer<const MLVec4F> src)
	{
		src += count - 1;
		
		for (int i = 0; i < count; ++i)
			BlendFunctions::blendFunc(*dst++, *src--);
	}
	
	void blendReversed(int count, MLPointer<MLVec4F> dst, MLPointer<const MLVec4F> src, MLPointer<const MLVec4F> masks)
	{
		src += count - 1;
		
		for (int i = 0; i < count; ++i)
			BlendFunctions::blendFunc(*dst++, *src-- * (masks++)->a);
	}

	void blendReversed(int count, MLPointer<MLVec4F> dst, MLPointer<const MLVec4F> src, MLPointer<const float> opacities)
	{
		src += count - 1;
		
		for (int i = 0; i < count; ++i)
			BlendFunctions::blendFunc(*dst++, *src-- * *opacities++);
	}

	void blendReversed(int count, MLPointer<MLVec4F> dst, MLPointer<const MLVec4F> src, const MLVec4F &mask)
	{
		src += count - 1;
		
		for (int i = 0; i < count; ++i)
			BlendFunctions::blendFunc(*dst++, *src-- * mask.a);
	}

	void blendReversed(int count, MLPointer<MLVec4F> dst, MLPointer<const MLVec4F> src, float opacity)
	{
		src += count - 1;
		
		for (int i = 0; i < count; ++i)
			BlendFunctions::blendFunc(*dst++, *src-- * opacity);
	}
	
	TileStates tileOp(TileStates states)
	{
		return BlendFunctions::tileOpFunc(states);
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
