#include "mlblendop.h"

// Pixel blend mode based on SVG compositing specification

class BlendFunctionsClear
{
public:
	static void blend(MLVec4F &dst, const MLVec4F &src)
	{
		Q_UNUSED(src);
		dst = MLVec4F(0);
	}
	
	static MLBlendOp::TileCombination tileRequirement(MLBlendOp::TileCombination states)
	{
		Q_UNUSED(states);
		return MLBlendOp::NoTile;
	}
};

class BlendFunctionsSource
{
public:
	static void blend(MLVec4F &dst, const MLVec4F &src)
	{
		dst = src;
	}
	
	static MLBlendOp::TileCombination tileRequirement(MLBlendOp::TileCombination states)
	{
		switch (states)
		{
		case MLBlendOp::TileBoth:
			return MLBlendOp::TileSource;
		case MLBlendOp::TileSource:
			return MLBlendOp::TileSource;
		case MLBlendOp::TileDestination:
			return MLBlendOp::NoTile;
		default:
			return MLBlendOp::NoTile;
		}
	}
};

class BlendFunctionsDestination
{
public:
	static void blend(MLVec4F &dst, const MLVec4F &src)
	{
		Q_UNUSED(dst); Q_UNUSED(src);
	}
	
	static MLBlendOp::TileCombination tileRequirement(MLBlendOp::TileCombination states)
	{
		switch (states)
		{
		case MLBlendOp::TileBoth:
			return MLBlendOp::TileDestination;
		case MLBlendOp::TileSource:
			return MLBlendOp::NoTile;
		case MLBlendOp::TileDestination:
			return MLBlendOp::TileDestination;
		default:
			return MLBlendOp::NoTile;
		}
	}
};

class BlendFunctionsSourceOver
{
public:
	static void blend(MLVec4F &dst, const MLVec4F &src)
	{
		dst = src + (1.0f - src.a) * dst;
	}
	
	static MLBlendOp::TileCombination tileRequirement(MLBlendOp::TileCombination states)
	{
		return states;
	}
};

class BlendFunctionsDestinationOver
{
public:
	static void blend(MLVec4F &dst, const MLVec4F &src)
	{
		dst = dst + (1.0f - dst.a) * src;
	}
	
	static MLBlendOp::TileCombination tileRequirement(MLBlendOp::TileCombination states)
	{
		return states;
	}
};

class BlendFunctionsSourceIn
{
public:
	static void blend(MLVec4F &dst, const MLVec4F &src)
	{
		dst = dst.a * src;
	}
	
	static MLBlendOp::TileCombination tileRequirement(MLBlendOp::TileCombination states)
	{
		switch (states)
		{
		case MLBlendOp::TileBoth:
			return MLBlendOp::TileBoth;
		case MLBlendOp::TileSource:
			return MLBlendOp::NoTile;
		case MLBlendOp::TileDestination:
			return MLBlendOp::NoTile;
		default:
			return MLBlendOp::NoTile;
		}
	}
};

class BlendFunctionsDestinationIn
{
public:
	static void blend(MLVec4F &dst, const MLVec4F &src)
	{
		dst = src.a * dst;
	}
	
	static MLBlendOp::TileCombination tileRequirement(MLBlendOp::TileCombination states)
	{
		switch (states)
		{
		case MLBlendOp::TileBoth:
			return MLBlendOp::TileBoth;
		case MLBlendOp::TileSource:
			return MLBlendOp::NoTile;
		case MLBlendOp::TileDestination:
			return MLBlendOp::NoTile;
		default:
			return MLBlendOp::NoTile;
		}
	}
};

class BlendFunctionsSourceOut
{
public:
	static void blend(MLVec4F &dst, const MLVec4F &src)
	{
		dst = (1.0f - dst.a) * src;
	}
	
	static MLBlendOp::TileCombination tileRequirement(MLBlendOp::TileCombination states)
	{
		switch (states)
		{
		case MLBlendOp::TileBoth:
			return MLBlendOp::TileBoth;
		case MLBlendOp::TileSource:
			return MLBlendOp::TileSource;
		case MLBlendOp::TileDestination:
			return MLBlendOp::NoTile;
		default:
			return MLBlendOp::NoTile;
		}
	}
};

class BlendFunctionsDestinationOut
{
public:
	static void blend(MLVec4F &dst, const MLVec4F &src)
	{
		dst = (1.0f - src.a) * dst;
	}
	
	static MLBlendOp::TileCombination tileRequirement(MLBlendOp::TileCombination states)
	{
		switch (states)
		{
		case MLBlendOp::TileBoth:
			return MLBlendOp::TileBoth;
		case MLBlendOp::TileSource:
			return MLBlendOp::NoTile;
		case MLBlendOp::TileDestination:
			return MLBlendOp::TileDestination;
		default:
			return MLBlendOp::NoTile;
		}
	}
};

class BlendFunctionsSourceAtop
{
public:
	static void blend(MLVec4F &dst, const MLVec4F &src)
	{
		dst = dst.a * src + (1.0f - src.a) * dst;
	}
	
	static MLBlendOp::TileCombination tileRequirement(MLBlendOp::TileCombination states)
	{
		switch (states)
		{
		case MLBlendOp::TileBoth:
			return MLBlendOp::TileBoth;
		case MLBlendOp::TileSource:
			return MLBlendOp::NoTile;
		case MLBlendOp::TileDestination:
			return MLBlendOp::TileDestination;
		default:
			return MLBlendOp::NoTile;
		}
	}
};

class BlendFunctionsDestinationAtop
{
public:
	static void blend(MLVec4F &dst, const MLVec4F &src)
	{
		dst = src.a * dst + (1.0f - dst.a) * src;
	}
	
	static MLBlendOp::TileCombination tileRequirement(MLBlendOp::TileCombination states)
	{
		switch (states)
		{
		case MLBlendOp::TileBoth:
			return MLBlendOp::TileBoth;
		case MLBlendOp::TileSource:
			return MLBlendOp::TileSource;
		case MLBlendOp::TileDestination:
			return MLBlendOp::NoTile;
		default:
			return MLBlendOp::NoTile;
		}
	}
};

class BlendFunctionsXor
{
public:
	static void blend(MLVec4F &dst, const MLVec4F &src)
	{
		dst = (1.0f - dst.a) * src + (1.0f - src.a) * dst;
	}
	
	static MLBlendOp::TileCombination tileRequirement(MLBlendOp::TileCombination states)
	{
		switch (states)
		{
		case MLBlendOp::TileBoth:
			return MLBlendOp::TileBoth;
		case MLBlendOp::TileSource:
			return MLBlendOp::TileSource;
		case MLBlendOp::TileDestination:
			return MLBlendOp::TileDestination;
		default:
			return MLBlendOp::NoTile;
		}
	}
};

class BlendFunctionsPlus
{
public:
	static void blend(MLVec4F &dst, const MLVec4F &src)
	{
		dst = mlBound(0, dst + src , 1);
	}
	
	static MLBlendOp::TileCombination tileRequirement(MLBlendOp::TileCombination states)
	{
		return states;
	}
};

class BlendFunctionsMultiply
{
public:
	static void blend(MLVec4F &dst, const MLVec4F &src)
	{
		dst = src * dst + src * (1.0f - dst.a) + dst * (1.0f - src.a);
	}
	
	static MLBlendOp::TileCombination tileRequirement(MLBlendOp::TileCombination states)
	{
		return states;
	}
};



class BlendFunctionsScreen
{
public:
	static void blend(MLVec4F &dst, const MLVec4F &src)
	{
		dst = src + dst - src * dst;
	}
	
	static MLBlendOp::TileCombination tileRequirement(MLBlendOp::TileCombination states)
	{
		return states;
	}
};

class BlendFunctionsOverlay
{
public:
	static void blend(MLVec4F &dst, const MLVec4F &src)
	{
		if (src.a == 0) return;
		if (dst.a == 0) { dst = src; return; }
		
		MLVec4F d, d1, d2;
		
		d = src * (1.0f - dst.a) + dst * (1.0f - src.a);
		d1 = 2.0f * src * dst;
		d2 = src.a * dst.a - 2.0f * (dst.a - dst) * (src.a - src);
		
		MLVec4I32 comp = MLVec4F::lessThanOrEqual(2.0f * dst, dst.a);
		
		d.r += comp.r ? d1.r : d2.r;
		d.g += comp.g ? d1.g : d2.g;
		d.b += comp.b ? d1.b : d2.b;
		d.a += src.a * dst.a;
		
		dst = d;
	}
	
	static MLBlendOp::TileCombination tileRequirement(MLBlendOp::TileCombination states)
	{
		return states;
	}
};

class BlendFunctionsDarken
{
public:
	static void blend(MLVec4F &dst, const MLVec4F &src)
	{
		if (src.a == 0) return;
		if (dst.a == 0) { dst = src; return; }
		
		MLVec4F d, ds;
		ds = src + (1.0f - src.a) * dst;
		d = dst + (1.0f - dst.a) * src;
		
		MLVec4I32 comp = MLVec4F::lessThan(src * dst.a, dst * src.a);
		
		if (comp.r) d.r = ds.r;
		if (comp.g) d.g = ds.g;
		if (comp.b) d.b = ds.b;
		
		dst = d;
	}
	
	static MLBlendOp::TileCombination tileRequirement(MLBlendOp::TileCombination states)
	{
		return states;
	}
};

class BlendFunctionsLighten
{
public:
	static void blend(MLVec4F &dst, const MLVec4F &src)
	{
		if (src.a == 0) return;
		if (dst.a == 0) { dst = src; return; }
		
		MLVec4F d, ds;
		ds = src + (1.0f - src.a) * dst;
		d = dst + (1.0f - dst.a) * src;
		
		MLVec4I32 comp = MLVec4F::greaterThan(src * dst.a, dst * src.a);
		
		if (comp.r) d.r = ds.r;
		if (comp.g) d.g = ds.g;
		if (comp.b) d.b = ds.b;
		
		dst = d;
	}
	
	static MLBlendOp::TileCombination tileRequirement(MLBlendOp::TileCombination states)
	{
		return states;
	}
};

class BlendFunctionsColorDodge
{
public:
	static void blend(MLVec4F &dst, const MLVec4F &src)
	{
		if (src.a == 0) return;
		if (dst.a == 0) { dst = src; return; }
		
		MLVec4F d, d1, d2;
		d1 = src * (1.0f - dst.a);
		d2 = d1 + (src.a * dst.a) + dst * (1.0f - src.a);
		d = (src.a * dst.a) * mlMin(1.0f, dst * src.a / (dst.a * (src.a - src)));
		
		MLVec4I32 compS = MLVec4F::equal(src, src.a);
		MLVec4I32 compD = MLVec4F::equal(dst, 0.0f);
		
		if (compS.r)
			d.r = compD.r ? d1.r : d2.r;
		if (compS.g)
			d.g = compD.g ? d1.g : d2.g;
		if (compS.b)
			d.b = compD.b ? d1.b : d2.b;
		
		d.a = src.a + dst.a - src.a * dst.a;
		
		dst = d;
	}
	
	static MLBlendOp::TileCombination tileRequirement(MLBlendOp::TileCombination states)
	{
		return states;
	}
};

class BlendFunctionsColorBurn
{
public:
	static void blend(MLVec4F &dst, const MLVec4F &src)
	{
		if (src.a == 0) return;
		if (dst.a == 0) { dst = src; return; }
		
		MLVec4F d, d1, d2;
		
		d2 = dst * (1.0f - src.a);
		d1 = (src.a * dst.a) + d2;
		d = d1 + (src.a * dst.a) * mlMin(1.0f, (dst.a * dst) * src.a / (dst.a * src)) + src * (1.0f - dst.a);
		
		MLVec4I32 compS = MLVec4F::equal(src, 0.0f);
		MLVec4I32 compD = MLVec4F::equal(dst, dst.a);
		
		if (compS.r)
			d.r = compD.r ? d1.r : d2.r;
		if (compS.g)
			d.g = compD.g ? d1.g : d2.g;
		if (compS.b)
			d.b = compD.b ? d1.b : d2.b;
		
		d.a = src.a + dst.a - src.a * dst.a;
		
		dst = d;
	}
	
	static MLBlendOp::TileCombination tileRequirement(MLBlendOp::TileCombination states)
	{
		return states;
	}
};

class BlendFunctionsHardLight
{
public:
	static void blend(MLVec4F &dst, const MLVec4F &src)
	{
		if (src.a == 0) return;
		if (dst.a == 0) { dst = src; return; }
		
		MLVec4F d, d1, d2;
		
		d = src * (1.0f - dst.a) + dst * (1.0f - src.a);
		d1 = 2.0f * src;
		d2 = (src.a * dst.a) - 2.0f * (dst.a - dst) * (src.a - src);
		
		MLVec4I32 comp = MLVec4F::lessThanOrEqual(2.0f * src, src.a);
		
		d.r += comp.r ? d1.r : d2.r;
		d.g += comp.g ? d1.g : d2.g;
		d.b += comp.b ? d1.b : d2.b;
		d.a += src.a * dst.a;
		
		dst = d;
	}
	
	static MLBlendOp::TileCombination tileRequirement(MLBlendOp::TileCombination states)
	{
		return states;
	}
};

class BlendFunctionsSoftLight
{
public:
	static void blend(MLVec4F &dst, const MLVec4F &src)
	{
		MLVec4F m, f, d0, d1, d2, d3;
		m = dst / dst.a;
		f = 2.0f * src - src.a;
		d0 = src * (1.0f - dst.a) + dst;
		d1 = dst * f * (1 - m) + d0;
		d2 = dst.a * f * (4.0f * m * (4.0f * m + 1.0f) * (m - 1.0f) + 7.0f * m) + d0;
		d3 = dst.a * f * (mlSqrt(m) - m) + d0;
		
		MLVec4I32 compS = MLVec4F::lessThanOrEqual(2.0f * src, src.a);
		MLVec4I32 compD = MLVec4F::lessThanOrEqual(4.0f * dst, dst.a);
		
		dst.a = d0.a;
		
		if (compS.r)
			dst.r = d1.r;
		else
			dst.r = compD.r ? d2.r : d3.r;
		
		if (compS.g)
			dst.g = d1.g;
		else
			dst.g = compD.g ? d2.g : d3.b;
		
		if (compS.b)
			dst.b = d1.b;
		else
			dst.b = compD.b ? d2.b : d3.b;
	}
	
	static MLBlendOp::TileCombination tileRequirement(MLBlendOp::TileCombination states)
	{
		return states;
	}
};

class BlendFunctionsDifference
{
public:
	static void blend(MLVec4F &dst, const MLVec4F &src)
	{
		MLVec4F d = src + dst - 2.0f * mlMin(src * dst.a, dst * src.a);
		d.a += src.a * dst.a;
		dst = d;
	}
	
	static MLBlendOp::TileCombination tileRequirement(MLBlendOp::TileCombination states)
	{
		return states;
	}
};

class BlendFunctionsExclusion
{
public:
	static void blend(MLVec4F &dst, const MLVec4F &src)
	{
		MLVec4F d = (src * dst.a + dst * src.a - (2.0f * src.a * dst.a)) + src * (1.0f - dst.a) + dst * (1.0f - src.a);
		d.a += src.a * dst.a;
		dst = d;
	}
	
	static MLBlendOp::TileCombination tileRequirement(MLBlendOp::TileCombination states)
	{
		return states;
	}
};

MLBlendOpDictionary::MLBlendOpDictionary()
{
	_blendOps[ML::BlendModeClear] = new MLTemplateBlendOp<BlendFunctionsClear>;
	_blendOps[ML::BlendModeSource] = new MLTemplateBlendOp<BlendFunctionsSource>;
	_blendOps[ML::BlendModeDestination] = new MLTemplateBlendOp<BlendFunctionsDestination>;
	_blendOps[ML::BlendModeSourceOver] = new MLTemplateBlendOp<BlendFunctionsSourceOver>;
	_blendOps[ML::BlendModeDestinationOver] = new MLTemplateBlendOp<BlendFunctionsDestinationOver>;
	_blendOps[ML::BlendModeSourceIn] = new MLTemplateBlendOp<BlendFunctionsSourceIn>;
	_blendOps[ML::BlendModeDestinationIn] = new MLTemplateBlendOp<BlendFunctionsDestinationIn>;
	_blendOps[ML::BlendModeSourceOut] = new MLTemplateBlendOp<BlendFunctionsSourceOut>;
	_blendOps[ML::BlendModeDestinationOut] = new MLTemplateBlendOp<BlendFunctionsDestinationOut>;
	_blendOps[ML::BlendModeSourceAtop] = new MLTemplateBlendOp<BlendFunctionsSourceAtop>;
	_blendOps[ML::BlendModeDestinationAtop] = new MLTemplateBlendOp<BlendFunctionsDestinationAtop>;
	_blendOps[ML::BlendModeXor] = new MLTemplateBlendOp<BlendFunctionsXor>;
	
	_blendOps[ML::BlendModeNormal] = _blendOps.value(ML::BlendModeSourceOver);
	_blendOps[ML::BlendModePlus] = new MLTemplateBlendOp<BlendFunctionsPlus>;
	_blendOps[ML::BlendModeMultiply] = new MLTemplateBlendOp<BlendFunctionsMultiply>;
	_blendOps[ML::BlendModeScreen] = new MLTemplateBlendOp<BlendFunctionsScreen>;
	_blendOps[ML::BlendModeOverlay] = new MLTemplateBlendOp<BlendFunctionsOverlay>;
	_blendOps[ML::BlendModeDarken] = new MLTemplateBlendOp<BlendFunctionsDarken>;
	_blendOps[ML::BlendModeLighten] = new MLTemplateBlendOp<BlendFunctionsLighten>;
	_blendOps[ML::BlendModeColorDodge] = new MLTemplateBlendOp<BlendFunctionsColorDodge>;
	_blendOps[ML::BlendModeColorBurn] = new MLTemplateBlendOp<BlendFunctionsColorBurn>;
	_blendOps[ML::BlendModeHardLight] = new MLTemplateBlendOp<BlendFunctionsHardLight>;
	_blendOps[ML::BlendModeSoftLight] = new MLTemplateBlendOp<BlendFunctionsSoftLight>;
	_blendOps[ML::BlendModeDifference] = new MLTemplateBlendOp<BlendFunctionsDifference>;
	_blendOps[ML::BlendModeExclusion] = new MLTemplateBlendOp<BlendFunctionsExclusion>;
}

MLBlendOpDictionary BlendOpDictionary;

MLBlendOpDictionary *mlBlendOpDictionary()
{
	return &BlendOpDictionary;
}

