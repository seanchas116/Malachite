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
	
	virtual void blend(int count, MLFastArgbF *dst, const MLFastArgbF *src) = 0;
	virtual void blend(int count, MLFastArgbF *dst, const MLFastArgbF *src, const MLFastArgbF *masks) = 0;
	virtual void blend(int count, MLFastArgbF *dst, const MLFastArgbF *src, const float *opacities) = 0;
	virtual void blend(int count, MLFastArgbF *dst, const MLFastArgbF *src, const MLFastArgbF &mask) = 0;
	virtual void blend(int count, MLFastArgbF *dst, const MLFastArgbF *src, float opacity) = 0;
	virtual void blend(int count, MLFastArgbF *dst, const MLFastArgbF &src) = 0;
	virtual void blend(int count, MLFastArgbF *dst, const MLFastArgbF &src, const MLFastArgbF *masks) = 0;
	virtual void blend(int count, MLFastArgbF *dst, const MLFastArgbF &src, const float *opacities) = 0;
	virtual void blend(int count, MLFastArgbF *dst, const MLFastArgbF &src, const MLFastArgbF &mask) = 0;
	virtual void blend(int count, MLFastArgbF *dst, const MLFastArgbF &src, float opacity) = 0;
	
	virtual TileStates tileOp(TileStates states) = 0;
};

template <void (*blendFunc)(MLFastArgbF *dst, const MLFastArgbF *src), MLBlendOp::TileStates (*tileOpFunc)(MLBlendOp::TileStates states)>
class MLTemplateBlendOp : public MLBlendOp
{
public:
	void blend(int count, MLFastArgbF *dst, const MLFastArgbF *src)
	{
		for (int i = 0; i < count; ++i)
		{
			blendFunc(dst++, src++);
		}
	}
	
	void blend(int count, MLFastArgbF *dst, const MLFastArgbF *src, const MLFastArgbF *masks)
	{
		for (int i = 0; i < count; ++i)
		{
			MLFastArgbF c;
			c.v = src->v * mlFloatToVector(masks->a);
			src++;
			masks++;
			blendFunc(dst++, &c);
		}
	}

	void blend(int count, MLFastArgbF *dst, const MLFastArgbF *src, const float *opacities)
	{
		for (int i = 0; i < count; ++i)
		{
			MLFastArgbF c;
			c.v = src->v * mlFloatToVector(*opacities);
			src++;
			opacities++;
			blendFunc(dst++, &c);
		}
	}

	void blend(int count, MLFastArgbF *dst, const MLFastArgbF *src, const MLFastArgbF &mask)
	{
		for (int i = 0; i < count; ++i)
		{
			MLFastArgbF c;
			c.v = src->v * mlFloatToVector(mask.a);
			src++;
			blendFunc(dst++, &c);
		}
	}

	void blend(int count, MLFastArgbF *dst, const MLFastArgbF *src, float opacity)
	{
		for (int i = 0; i < count; ++i)
		{
			MLFastArgbF c;
			c.v = src->v * mlFloatToVector(opacity);
			src++;
			blendFunc(dst++, &c);
		}
	}
	
	void blend(int count, MLFastArgbF *dst, const MLFastArgbF &src)
	{
		for (int i = 0; i < count; ++i)
		{
			blendFunc(dst++, &src);
		}
	}
	
	void blend(int count, MLFastArgbF *dst, const MLFastArgbF &src, const MLFastArgbF *masks)
	{
		for (int i = 0; i < count; ++i)
		{
			MLFastArgbF c;
			c.v = src.v * mlFloatToVector((masks++)->a);
			blendFunc(dst++, &c);
		}
	}
	
	void blend(int count, MLFastArgbF *dst, const MLFastArgbF &src, const float *opacities)
	{
		for (int i = 0; i < count; ++i)
		{
			MLFastArgbF c;
			c.v = src.v * mlFloatToVector(*opacities++);
			blendFunc(dst++, &c);
		}
	} 
	
	void blend(int count, MLFastArgbF *dst, const MLFastArgbF &src, const MLFastArgbF &mask)
	{
		MLFastArgbF c;
		c.v = src.v * mlFloatToVector(mask.a);
		for (int i = 0; i < count; ++i)
		{
			blendFunc(dst++, &c);
		}
	}
	
	void blend(int count, MLFastArgbF *dst, const MLFastArgbF &src, float opacity)
	{
		MLFastArgbF c;
		c.v = src.v * mlFloatToVector(opacity);
		for (int i = 0; i < count; ++i)
		{
			blendFunc(dst++, &c);
		}
	}
	
	TileStates tileOp(TileStates states)
	{
		return tileOpFunc(states);
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
