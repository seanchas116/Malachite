#include "blendop.h"

// Pixel blend mode based on SVG compositing specification

namespace Malachite
{

class BlendFunctionsClear
{
public:
	static void blend(Vec4F &dst, const Vec4F &src)
	{
		Q_UNUSED(src);
		dst = Vec4F(0);
	}
	
	static BlendOp::TileCombination tileRequirement(BlendOp::TileCombination states)
	{
		Q_UNUSED(states);
		return BlendOp::NoTile;
	}
};

class BlendFunctionsSource
{
public:
	static void blend(Vec4F &dst, const Vec4F &src)
	{
		dst = src;
	}
	
	static BlendOp::TileCombination tileRequirement(BlendOp::TileCombination states)
	{
		switch (states)
		{
		case BlendOp::TileBoth:
			return BlendOp::TileSource;
		case BlendOp::TileSource:
			return BlendOp::TileSource;
		case BlendOp::TileDestination:
			return BlendOp::NoTile;
		default:
			return BlendOp::NoTile;
		}
	}
};

class BlendFunctionsDestination
{
public:
	static void blend(Vec4F &dst, const Vec4F &src)
	{
		Q_UNUSED(dst); Q_UNUSED(src);
	}
	
	static BlendOp::TileCombination tileRequirement(BlendOp::TileCombination states)
	{
		switch (states)
		{
		case BlendOp::TileBoth:
			return BlendOp::TileDestination;
		case BlendOp::TileSource:
			return BlendOp::NoTile;
		case BlendOp::TileDestination:
			return BlendOp::TileDestination;
		default:
			return BlendOp::NoTile;
		}
	}
};

class BlendFunctionsSourceOver
{
public:
	static void blend(Vec4F &dst, const Vec4F &src)
	{
		dst = src + (1.0f - src.a) * dst;
	}
	
	static BlendOp::TileCombination tileRequirement(BlendOp::TileCombination states)
	{
		return states;
	}
};

class BlendFunctionsDestinationOver
{
public:
	static void blend(Vec4F &dst, const Vec4F &src)
	{
		dst = dst + (1.0f - dst.a) * src;
	}
	
	static BlendOp::TileCombination tileRequirement(BlendOp::TileCombination states)
	{
		return states;
	}
};

class BlendFunctionsSourceIn
{
public:
	static void blend(Vec4F &dst, const Vec4F &src)
	{
		dst = dst.a * src;
	}
	
	static BlendOp::TileCombination tileRequirement(BlendOp::TileCombination states)
	{
		switch (states)
		{
		case BlendOp::TileBoth:
			return BlendOp::TileBoth;
		case BlendOp::TileSource:
			return BlendOp::NoTile;
		case BlendOp::TileDestination:
			return BlendOp::NoTile;
		default:
			return BlendOp::NoTile;
		}
	}
};

class BlendFunctionsDestinationIn
{
public:
	static void blend(Vec4F &dst, const Vec4F &src)
	{
		dst = src.a * dst;
	}
	
	static BlendOp::TileCombination tileRequirement(BlendOp::TileCombination states)
	{
		switch (states)
		{
		case BlendOp::TileBoth:
			return BlendOp::TileBoth;
		case BlendOp::TileSource:
			return BlendOp::NoTile;
		case BlendOp::TileDestination:
			return BlendOp::NoTile;
		default:
			return BlendOp::NoTile;
		}
	}
};

class BlendFunctionsSourceOut
{
public:
	static void blend(Vec4F &dst, const Vec4F &src)
	{
		dst = (1.0f - dst.a) * src;
	}
	
	static BlendOp::TileCombination tileRequirement(BlendOp::TileCombination states)
	{
		switch (states)
		{
		case BlendOp::TileBoth:
			return BlendOp::TileBoth;
		case BlendOp::TileSource:
			return BlendOp::TileSource;
		case BlendOp::TileDestination:
			return BlendOp::NoTile;
		default:
			return BlendOp::NoTile;
		}
	}
};

class BlendFunctionsDestinationOut
{
public:
	static void blend(Vec4F &dst, const Vec4F &src)
	{
		dst = (1.0f - src.a) * dst;
	}
	
	static BlendOp::TileCombination tileRequirement(BlendOp::TileCombination states)
	{
		switch (states)
		{
		case BlendOp::TileBoth:
			return BlendOp::TileBoth;
		case BlendOp::TileSource:
			return BlendOp::NoTile;
		case BlendOp::TileDestination:
			return BlendOp::TileDestination;
		default:
			return BlendOp::NoTile;
		}
	}
};

class BlendFunctionsSourceAtop
{
public:
	static void blend(Vec4F &dst, const Vec4F &src)
	{
		dst = dst.a * src + (1.0f - src.a) * dst;
	}
	
	static BlendOp::TileCombination tileRequirement(BlendOp::TileCombination states)
	{
		switch (states)
		{
		case BlendOp::TileBoth:
			return BlendOp::TileBoth;
		case BlendOp::TileSource:
			return BlendOp::NoTile;
		case BlendOp::TileDestination:
			return BlendOp::TileDestination;
		default:
			return BlendOp::NoTile;
		}
	}
};

class BlendFunctionsDestinationAtop
{
public:
	static void blend(Vec4F &dst, const Vec4F &src)
	{
		dst = src.a * dst + (1.0f - dst.a) * src;
	}
	
	static BlendOp::TileCombination tileRequirement(BlendOp::TileCombination states)
	{
		switch (states)
		{
		case BlendOp::TileBoth:
			return BlendOp::TileBoth;
		case BlendOp::TileSource:
			return BlendOp::TileSource;
		case BlendOp::TileDestination:
			return BlendOp::NoTile;
		default:
			return BlendOp::NoTile;
		}
	}
};

class BlendFunctionsXor
{
public:
	static void blend(Vec4F &dst, const Vec4F &src)
	{
		dst = (1.0f - dst.a) * src + (1.0f - src.a) * dst;
	}
	
	static BlendOp::TileCombination tileRequirement(BlendOp::TileCombination states)
	{
		switch (states)
		{
		case BlendOp::TileBoth:
			return BlendOp::TileBoth;
		case BlendOp::TileSource:
			return BlendOp::TileSource;
		case BlendOp::TileDestination:
			return BlendOp::TileDestination;
		default:
			return BlendOp::NoTile;
		}
	}
};

class BlendFunctionsPlus
{
public:
	static void blend(Vec4F &dst, const Vec4F &src)
	{
		dst = vecBound(0, dst + src , 1);
	}
	
	static BlendOp::TileCombination tileRequirement(BlendOp::TileCombination states)
	{
		return states;
	}
};

class BlendFunctionsMultiply
{
public:
	static void blend(Vec4F &dst, const Vec4F &src)
	{
		dst = src * dst + src * (1.0f - dst.a) + dst * (1.0f - src.a);
	}
	
	static BlendOp::TileCombination tileRequirement(BlendOp::TileCombination states)
	{
		switch (states)
		{
		case BlendOp::TileBoth:
			return BlendOp::TileBoth;
		case BlendOp::TileSource:
		case BlendOp::TileDestination:
		default:
			return BlendOp::NoTile;
		}
	}
};



class BlendFunctionsScreen
{
public:
	static void blend(Vec4F &dst, const Vec4F &src)
	{
		dst = src + dst - src * dst;
	}
	
	static BlendOp::TileCombination tileRequirement(BlendOp::TileCombination states)
	{
		return states;
	}
};

class BlendFunctionsOverlay
{
public:
	static void blend(Vec4F &dst, const Vec4F &src)
	{
		if (src.a == 0) return;
		if (dst.a == 0) { dst = src; return; }
		
		Vec4F d, d1, d2;
		
		d = src * (1.0f - dst.a) + dst * (1.0f - src.a);
		d1 = 2.0f * src * dst;
		d2 = src.a * dst.a - 2.0f * (dst.a - dst) * (src.a - src);
		
		Vec4I32 comp = Vec4F::lessThanOrEqual(2.0f * dst, dst.a);
		
		d.r += comp.r ? d1.r : d2.r;
		d.g += comp.g ? d1.g : d2.g;
		d.b += comp.b ? d1.b : d2.b;
		d.a += src.a * dst.a;
		
		dst = d;
	}
	
	static BlendOp::TileCombination tileRequirement(BlendOp::TileCombination states)
	{
		return states;
	}
};

class BlendFunctionsDarken
{
public:
	static void blend(Vec4F &dst, const Vec4F &src)
	{
		if (src.a == 0) return;
		if (dst.a == 0) { dst = src; return; }
		
		Vec4F d, ds;
		ds = src + (1.0f - src.a) * dst;
		d = dst + (1.0f - dst.a) * src;
		
		Vec4I32 comp = Vec4F::lessThan(src * dst.a, dst * src.a);
		
		if (comp.r) d.r = ds.r;
		if (comp.g) d.g = ds.g;
		if (comp.b) d.b = ds.b;
		
		dst = d;
	}
	
	static BlendOp::TileCombination tileRequirement(BlendOp::TileCombination states)
	{
		return states;
	}
};

class BlendFunctionsLighten
{
public:
	static void blend(Vec4F &dst, const Vec4F &src)
	{
		if (src.a == 0) return;
		if (dst.a == 0) { dst = src; return; }
		
		Vec4F d, ds;
		ds = src + (1.0f - src.a) * dst;
		d = dst + (1.0f - dst.a) * src;
		
		Vec4I32 comp = Vec4F::greaterThan(src * dst.a, dst * src.a);
		
		if (comp.r) d.r = ds.r;
		if (comp.g) d.g = ds.g;
		if (comp.b) d.b = ds.b;
		
		dst = d;
	}
	
	static BlendOp::TileCombination tileRequirement(BlendOp::TileCombination states)
	{
		return states;
	}
};

class BlendFunctionsColorDodge
{
public:
	static void blend(Vec4F &dst, const Vec4F &src)
	{
		if (src.a == 0) return;
		if (dst.a == 0) { dst = src; return; }
		
		Vec4F d, d1, d2;
		d1 = src * (1.0f - dst.a);
		d2 = d1 + (src.a * dst.a) + dst * (1.0f - src.a);
		d = (src.a * dst.a) * vecMin(1.0f, dst * src.a / (dst.a * (src.a - src)));
		
		Vec4I32 compS = Vec4F::equal(src, src.a);
		Vec4I32 compD = Vec4F::equal(dst, 0.0f);
		
		if (compS.r)
			d.r = compD.r ? d1.r : d2.r;
		if (compS.g)
			d.g = compD.g ? d1.g : d2.g;
		if (compS.b)
			d.b = compD.b ? d1.b : d2.b;
		
		d.a = src.a + dst.a - src.a * dst.a;
		
		dst = d;
	}
	
	static BlendOp::TileCombination tileRequirement(BlendOp::TileCombination states)
	{
		return states;
	}
};

class BlendFunctionsColorBurn
{
public:
	static void blend(Vec4F &dst, const Vec4F &src)
	{
		if (src.a == 0) return;
		if (dst.a == 0) { dst = src; return; }
		
		Vec4F d, d1, d2;
		
		d2 = dst * (1.0f - src.a);
		d1 = (src.a * dst.a) + d2;
		d = d1 + (src.a * dst.a) * vecMin(1.0f, (dst.a * dst) * src.a / (dst.a * src)) + src * (1.0f - dst.a);
		
		Vec4I32 compS = Vec4F::equal(src, 0.0f);
		Vec4I32 compD = Vec4F::equal(dst, dst.a);
		
		if (compS.r)
			d.r = compD.r ? d1.r : d2.r;
		if (compS.g)
			d.g = compD.g ? d1.g : d2.g;
		if (compS.b)
			d.b = compD.b ? d1.b : d2.b;
		
		d.a = src.a + dst.a - src.a * dst.a;
		
		dst = d;
	}
	
	static BlendOp::TileCombination tileRequirement(BlendOp::TileCombination states)
	{
		return states;
	}
};

class BlendFunctionsHardLight
{
public:
	static void blend(Vec4F &dst, const Vec4F &src)
	{
		if (src.a == 0) return;
		if (dst.a == 0) { dst = src; return; }
		
		Vec4F d, d1, d2;
		
		d = src * (1.0f - dst.a) + dst * (1.0f - src.a);
		d1 = 2.0f * src;
		d2 = (src.a * dst.a) - 2.0f * (dst.a - dst) * (src.a - src);
		
		Vec4I32 comp = Vec4F::lessThanOrEqual(2.0f * src, src.a);
		
		d.r += comp.r ? d1.r : d2.r;
		d.g += comp.g ? d1.g : d2.g;
		d.b += comp.b ? d1.b : d2.b;
		d.a += src.a * dst.a;
		
		dst = d;
	}
	
	static BlendOp::TileCombination tileRequirement(BlendOp::TileCombination states)
	{
		return states;
	}
};

class BlendFunctionsSoftLight
{
public:
	static void blend(Vec4F &dst, const Vec4F &src)
	{
		if (src.a == 0) return;
		if (dst.a == 0) { dst = src; return; }
		
		Vec4F m, f, d0, d1, d2, d3;
		m = dst / dst.a;
		f = 2.0f * src - src.a;
		d0 = src * (1.0f - dst.a) + dst;
		d1 = dst * f * (1 - m) + d0;
		d2 = dst.a * f * (4.0f * m * (4.0f * m + 1.0f) * (m - 1.0f) + 7.0f * m) + d0;
		d3 = dst.a * f * (vecSqrt(m) - m) + d0;
		
		Vec4I32 compS = Vec4F::lessThanOrEqual(2.0f * src, src.a);
		Vec4I32 compD = Vec4F::lessThanOrEqual(4.0f * dst, dst.a);
		
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
	
	static BlendOp::TileCombination tileRequirement(BlendOp::TileCombination states)
	{
		return states;
	}
};

class BlendFunctionsDifference
{
public:
	static void blend(Vec4F &dst, const Vec4F &src)
	{
		if (src.a == 0) return;
		if (dst.a == 0) { dst = src; return; }
		
		Vec4F d = src + dst - 2.0f * vecMin(src * dst.a, dst * src.a);
		d.a += src.a * dst.a;
		dst = d;
	}
	
	static BlendOp::TileCombination tileRequirement(BlendOp::TileCombination states)
	{
		return states;
	}
};

class BlendFunctionsExclusion
{
public:
	static void blend(Vec4F &dst, const Vec4F &src)
	{
		if (src.a == 0) return;
		if (dst.a == 0) { dst = src; return; }
		
		Vec4F d = (src * dst.a + dst * src.a - (2.0f * src.a * dst.a)) + src * (1.0f - dst.a) + dst * (1.0f - src.a);
		d.a += src.a * dst.a;
		dst = d;
	}
	
	static BlendOp::TileCombination tileRequirement(BlendOp::TileCombination states)
	{
		return states;
	}
};

class BlendFunctionsDestinationPadding
{
public:
	
	static void blend(Vec4F &dst, const Vec4F &src)
	{
		if (dst.a == 1)
			return;
		
		float margin = 1 - dst.a;
		
		if (margin >= src.a)
		{
			dst += src;
			return;
		}
		
		dst += (margin / src.a) * src;
		dst.a = 1;
	}
	
	static BlendOp::TileCombination tileRequirement(BlendOp::TileCombination states)
	{
		return states;
	}
};

class BlendFunctionsSourcePadding
{
public:
	
	static void blend(Vec4F &dst, const Vec4F &src)
	{
		if (src.a == 1)
		{
			dst = src;
			return;
		}
		
		float margin = 1 - src.a;
		
		if (margin >= dst.a)
		{
			dst += src;
			return;
		}
		
		dst = (margin / dst.a) * dst + src;
		dst.a = 1;
	}
	
	static BlendOp::TileCombination tileRequirement(BlendOp::TileCombination states)
	{
		return states;
	}
};

BlendOpDictionary::BlendOpDictionary()
{
	_blendOps[Malachite::BlendModeClear] = new MLTemplateBlendOp<BlendFunctionsClear>;
	_blendOps[Malachite::BlendModeSource] = new MLTemplateBlendOp<BlendFunctionsSource>;
	_blendOps[Malachite::BlendModeDestination] = new MLTemplateBlendOp<BlendFunctionsDestination>;
	_blendOps[Malachite::BlendModeSourceOver] = new MLTemplateBlendOp<BlendFunctionsSourceOver>;
	_blendOps[Malachite::BlendModeDestinationOver] = new MLTemplateBlendOp<BlendFunctionsDestinationOver>;
	_blendOps[Malachite::BlendModeSourceIn] = new MLTemplateBlendOp<BlendFunctionsSourceIn>;
	_blendOps[Malachite::BlendModeDestinationIn] = new MLTemplateBlendOp<BlendFunctionsDestinationIn>;
	_blendOps[Malachite::BlendModeSourceOut] = new MLTemplateBlendOp<BlendFunctionsSourceOut>;
	_blendOps[Malachite::BlendModeDestinationOut] = new MLTemplateBlendOp<BlendFunctionsDestinationOut>;
	_blendOps[Malachite::BlendModeSourceAtop] = new MLTemplateBlendOp<BlendFunctionsSourceAtop>;
	_blendOps[Malachite::BlendModeDestinationAtop] = new MLTemplateBlendOp<BlendFunctionsDestinationAtop>;
	_blendOps[Malachite::BlendModeXor] = new MLTemplateBlendOp<BlendFunctionsXor>;
	
	_blendOps[Malachite::BlendModeNormal] = _blendOps.value(Malachite::BlendModeSourceOver);
	_blendOps[Malachite::BlendModePlus] = new MLTemplateBlendOp<BlendFunctionsPlus>;
	_blendOps[Malachite::BlendModeMultiply] = new MLTemplateBlendOp<BlendFunctionsMultiply>;
	_blendOps[Malachite::BlendModeScreen] = new MLTemplateBlendOp<BlendFunctionsScreen>;
	_blendOps[Malachite::BlendModeOverlay] = new MLTemplateBlendOp<BlendFunctionsOverlay>;
	_blendOps[Malachite::BlendModeDarken] = new MLTemplateBlendOp<BlendFunctionsDarken>;
	_blendOps[Malachite::BlendModeLighten] = new MLTemplateBlendOp<BlendFunctionsLighten>;
	_blendOps[Malachite::BlendModeColorDodge] = new MLTemplateBlendOp<BlendFunctionsColorDodge>;
	_blendOps[Malachite::BlendModeColorBurn] = new MLTemplateBlendOp<BlendFunctionsColorBurn>;
	_blendOps[Malachite::BlendModeHardLight] = new MLTemplateBlendOp<BlendFunctionsHardLight>;
	_blendOps[Malachite::BlendModeSoftLight] = new MLTemplateBlendOp<BlendFunctionsSoftLight>;
	_blendOps[Malachite::BlendModeDifference] = new MLTemplateBlendOp<BlendFunctionsDifference>;
	_blendOps[Malachite::BlendModeExclusion] = new MLTemplateBlendOp<BlendFunctionsExclusion>;
	
	_blendOps[Malachite::BlendModeSourcePadding] = new MLTemplateBlendOp<BlendFunctionsSourcePadding>;
	_blendOps[Malachite::BlendModeDestinationPadding] = new MLTemplateBlendOp<BlendFunctionsDestinationPadding>;
}

BlendOpDictionary _BlendOpDictionary;

BlendOpDictionary *blendOpDictionary()
{
	return &_BlendOpDictionary;
}

}

