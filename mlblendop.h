#ifndef MLBLENDOP_H
#define MLBLENDOP_H

#include "mlcolor.h"


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
	
	virtual void blend(int count, MLArgb *dst, const MLArgb *src) = 0;
	virtual void blend(int count, MLArgb *dst, const MLArgb *src, const MLArgb *masks) = 0;
	virtual void blend(int count, MLArgb *dst, const MLArgb *src, const float *opacities) = 0;
	virtual void blend(int count, MLArgb *dst, const MLArgb *src, const MLArgb &mask) = 0;
	virtual void blend(int count, MLArgb *dst, const MLArgb *src, float opacity) = 0;
	virtual void blend(int count, MLArgb *dst, const MLArgb &src) = 0;
	virtual void blend(int count, MLArgb *dst, const MLArgb &src, const MLArgb *masks) = 0;
	virtual void blend(int count, MLArgb *dst, const MLArgb &src, const float *opacities) = 0;
	virtual void blend(int count, MLArgb *dst, const MLArgb &src, const MLArgb &mask) = 0;
	virtual void blend(int count, MLArgb *dst, const MLArgb &src, float opacity) = 0;
	
	virtual TileStates tileOp(TileStates states) = 0;
};

/*
  
  example
  
class BlendFunctions
{
public:
	static void blendFunc(MLArgb *dst, const MLArgb *src)
	{
		
	}
	
	static MLBlendOp::TileStates tileOpFunc(MLBlendOp::TileStates states)
	{
		
	}
};

*/

template <typename BlendFunctions>
class MLTemplateBlendOp : public MLBlendOp
{
public:
	void blend(int count, MLArgb *dst, const MLArgb *src)
	{
		for (int i = 0; i < count; ++i)
		{
			BlendFunctions::blendFunc(*dst++, *src++);
		}
	}
	
	void blend(int count, MLArgb *dst, const MLArgb *src, const MLArgb *masks)
	{
		for (int i = 0; i < count; ++i)
		{
			MLArgb c;
			c.v = src->v * masks->a();
			src++;
			masks++;
			BlendFunctions::blendFunc(*dst++, c);
		}
	}

	void blend(int count, MLArgb *dst, const MLArgb *src, const float *opacities)
	{
		for (int i = 0; i < count; ++i)
		{
			MLArgb c;
			c.v = src->v * *opacities;
			src++;
			opacities++;
			BlendFunctions::blendFunc(*dst++, c);
		}
	}

	void blend(int count, MLArgb *dst, const MLArgb *src, const MLArgb &mask)
	{
		for (int i = 0; i < count; ++i)
		{
			MLArgb c;
			c.v = src->v * mask.a();
			src++;
			BlendFunctions::blendFunc(*dst++, c);
		}
	}

	void blend(int count, MLArgb *dst, const MLArgb *src, float opacity)
	{
		for (int i = 0; i < count; ++i)
		{
			MLArgb c;
			c.v = src->v * opacity;
			src++;
			BlendFunctions::blendFunc(*dst++, c);
		}
	}
	
	void blend(int count, MLArgb *dst, const MLArgb &src)
	{
		for (int i = 0; i < count; ++i)
		{
			BlendFunctions::blendFunc(*dst++, src);
		}
	}
	
	void blend(int count, MLArgb *dst, const MLArgb &src, const MLArgb *masks)
	{
		for (int i = 0; i < count; ++i)
		{
			MLArgb c;
			c.v = src.v * (masks++)->a();
			BlendFunctions::blendFunc(*dst++, c);
		}
	}
	
	void blend(int count, MLArgb *dst, const MLArgb &src, const float *opacities)
	{
		for (int i = 0; i < count; ++i)
		{
			MLArgb c;
			c.v = src.v * *opacities++;
			BlendFunctions::blendFunc(*dst++, c);
		}
	} 
	
	void blend(int count, MLArgb *dst, const MLArgb &src, const MLArgb &mask)
	{
		MLArgb c;
		c.v = src.v * mask.a();
		for (int i = 0; i < count; ++i)
		{
			BlendFunctions::blendFunc(*dst++, c);
		}
	}
	
	void blend(int count, MLArgb *dst, const MLArgb &src, float opacity)
	{
		MLArgb c;
		c.v = src.v * opacity;
		for (int i = 0; i < count; ++i)
		{
			BlendFunctions::blendFunc(*dst++, c);
		}
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
