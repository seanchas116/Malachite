#include "mlblendop.h"

// Pixel blend mode based on SVG compositing specification

class BlendFunctionsClear
{
public:
	static void blendFunc(MLArgb &dst, const MLArgb &src)
	{
		Q_UNUSED(src);
		dst.v = MLSimdF4(0);
	}
	
	static MLBlendOp::TileStates tileOpFunc(MLBlendOp::TileStates states)
	{
		Q_UNUSED(states);
		return MLBlendOp::NoTiles;
	}
};

class BlendFunctionsSource
{
public:
	static void blendFunc(MLArgb &dst, const MLArgb &src)
	{
		dst = src;
	}
	
	static MLBlendOp::TileStates tileOpFunc(MLBlendOp::TileStates states)
	{
		switch (states)
		{
		case MLBlendOp::TileBoth:
			return MLBlendOp::TileSource;
		case MLBlendOp::TileSource:
			return MLBlendOp::TileSource;
		case MLBlendOp::TileDestination:
			return MLBlendOp::NoTiles;
		default:
			return MLBlendOp::NoTiles;
		}
	}
};

class BlendFunctionsDestination
{
public:
	static void blendFunc(MLArgb &dst, const MLArgb &src)
	{
		Q_UNUSED(dst); Q_UNUSED(src);
	}
	
	static MLBlendOp::TileStates tileOpFunc(MLBlendOp::TileStates states)
	{
		switch (states)
		{
		case MLBlendOp::TileBoth:
			return MLBlendOp::TileDestination;
		case MLBlendOp::TileSource:
			return MLBlendOp::NoTiles;
		case MLBlendOp::TileDestination:
			return MLBlendOp::TileDestination;
		default:
			return MLBlendOp::NoTiles;
		}
	}
};

class BlendFunctionsSourceOver
{
public:
	static void blendFunc(MLArgb &dst, const MLArgb &src)
	{
		dst.v = src.v + (1.0f - src.a()) * dst.v;
	}
	
	static MLBlendOp::TileStates tileOpFunc(MLBlendOp::TileStates states)
	{
		return states;
	}
};

class BlendFunctionsDestinationOver
{
public:
	static void blendFunc(MLArgb &dst, const MLArgb &src)
	{
		dst.v = dst.v + (1.0f - dst.a()) * src.v;
	}
	
	static MLBlendOp::TileStates tileOpFunc(MLBlendOp::TileStates states)
	{
		return states;
	}
};

class BlendFunctionsSourceIn
{
public:
	static void blendFunc(MLArgb &dst, const MLArgb &src)
	{
		dst.v = dst.a() * src.v;
	}
	
	static MLBlendOp::TileStates tileOpFunc(MLBlendOp::TileStates states)
	{
		switch (states)
		{
		case MLBlendOp::TileBoth:
			return MLBlendOp::TileBoth;
		case MLBlendOp::TileSource:
			return MLBlendOp::NoTiles;
		case MLBlendOp::TileDestination:
			return MLBlendOp::NoTiles;
		default:
			return MLBlendOp::NoTiles;
		}
	}
};

class BlendFunctionsDestinationIn
{
public:
	static void blendFunc(MLArgb &dst, const MLArgb &src)
	{
		dst.v = src.a() * dst.v;
	}
	
	static MLBlendOp::TileStates tileOpFunc(MLBlendOp::TileStates states)
	{
		switch (states)
		{
		case MLBlendOp::TileBoth:
			return MLBlendOp::TileBoth;
		case MLBlendOp::TileSource:
			return MLBlendOp::NoTiles;
		case MLBlendOp::TileDestination:
			return MLBlendOp::NoTiles;
		default:
			return MLBlendOp::NoTiles;
		}
	}
};

class BlendFunctionsSourceOut
{
public:
	static void blendFunc(MLArgb &dst, const MLArgb &src)
	{
		dst.v = (1.0f - dst.a()) * src.v;
	}
	
	static MLBlendOp::TileStates tileOpFunc(MLBlendOp::TileStates states)
	{
		switch (states)
		{
		case MLBlendOp::TileBoth:
			return MLBlendOp::TileBoth;
		case MLBlendOp::TileSource:
			return MLBlendOp::TileSource;
		case MLBlendOp::TileDestination:
			return MLBlendOp::NoTiles;
		default:
			return MLBlendOp::NoTiles;
		}
	}
};

class BlendFunctionsDestinationOut
{
public:
	static void blendFunc(MLArgb &dst, const MLArgb &src)
	{
		dst.v = (1.0f - src.a()) * dst.v;
	}
	
	static MLBlendOp::TileStates tileOpFunc(MLBlendOp::TileStates states)
	{
		switch (states)
		{
		case MLBlendOp::TileBoth:
			return MLBlendOp::TileBoth;
		case MLBlendOp::TileSource:
			return MLBlendOp::NoTiles;
		case MLBlendOp::TileDestination:
			return MLBlendOp::TileDestination;
		default:
			return MLBlendOp::NoTiles;
		}
	}
};

class BlendFunctionsSourceAtop
{
public:
	static void blendFunc(MLArgb &dst, const MLArgb &src)
	{
		dst.v = dst.a() * src.v + (1.0f - src.a()) * dst.v;
	}
	
	static MLBlendOp::TileStates tileOpFunc(MLBlendOp::TileStates states)
	{
		switch (states)
		{
		case MLBlendOp::TileBoth:
			return MLBlendOp::TileBoth;
		case MLBlendOp::TileSource:
			return MLBlendOp::NoTiles;
		case MLBlendOp::TileDestination:
			return MLBlendOp::TileDestination;
		default:
			return MLBlendOp::NoTiles;
		}
	}
};

class BlendFunctionsDestinationAtop
{
public:
	static void blendFunc(MLArgb &dst, const MLArgb &src)
	{
		dst.v = src.a() * dst.v + (1.0f - dst.a()) * src.v;
	}
	
	static MLBlendOp::TileStates tileOpFunc(MLBlendOp::TileStates states)
	{
		switch (states)
		{
		case MLBlendOp::TileBoth:
			return MLBlendOp::TileBoth;
		case MLBlendOp::TileSource:
			return MLBlendOp::TileSource;
		case MLBlendOp::TileDestination:
			return MLBlendOp::NoTiles;
		default:
			return MLBlendOp::NoTiles;
		}
	}
};

class BlendFunctionsXor
{
public:
	static void blendFunc(MLArgb &dst, const MLArgb &src)
	{
		dst.v = (1.0f - dst.a()) * src.v + (1.0f - src.a()) * dst.v;
	}
	
	static MLBlendOp::TileStates tileOpFunc(MLBlendOp::TileStates states)
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
			return MLBlendOp::NoTiles;
		}
	}
};

class BlendFunctionsPlus
{
public:
	static void blendFunc(MLArgb &dst, const MLArgb &src)
	{
		dst.v = mlSimdBound(0, dst.v + src.v , 1);
	}
	
	static MLBlendOp::TileStates tileOpFunc(MLBlendOp::TileStates states)
	{
		return states;
	}
};

class BlendFunctionsMultiply
{
public:
	static void blendFunc(MLArgb &dst, const MLArgb &src)
	{
		dst.v = src.v * dst.v + src.v * (1.0f - dst.a()) + dst.v * (1.0f - src.a());
	}
	
	static MLBlendOp::TileStates tileOpFunc(MLBlendOp::TileStates states)
	{
		return states;
	}
};



class BlendFunctionsScreen
{
public:
	static void blendFunc(MLArgb &dst, const MLArgb &src)
	{
		dst.v = src.v + dst.v - src.v * dst.v;
	}
	
	static MLBlendOp::TileStates tileOpFunc(MLBlendOp::TileStates states)
	{
		return states;
	}
};

class BlendFunctionsOverlay
{
public:
	static void blendFunc(MLArgb &dst, const MLArgb &src)
	{
		if (src.a() == 0) return;
		if (dst.a() == 0) { dst = src; return; }
		
		MLArgb d, d1, d2;
		
		d.v = src.v * (1.0f - dst.a()) + dst.v * (1.0f - src.a());
		d1.v = 2.0f * src.v * dst.v;
		d2.v = src.a() * dst.a() - 2.0f * (dst.a() - dst.v) * (src.a() - src.v);
		
		MLSimd32I4 comp = mlSimdLessThanOrEqual(2.0f * dst.v, dst.a());
		
		d.r() += comp[MLGlobal::ArgbRed] ? d1.r() : d2.r();
		d.g() += comp[MLGlobal::ArgbGreen] ? d1.g() : d2.g();
		d.b() += comp[MLGlobal::ArgbBlue] ? d1.b() : d2.b();
		d.a() += src.a() * dst.a();
		
		dst = d;
	}
	
	static MLBlendOp::TileStates tileOpFunc(MLBlendOp::TileStates states)
	{
		return states;
	}
};

class BlendFunctionsDarken
{
public:
	static void blendFunc(MLArgb &dst, const MLArgb &src)
	{
		if (src.a() == 0) return;
		if (dst.a() == 0) { dst = src; return; }
		
		MLArgb d, ds;
		ds.v = src.v + (1.0f - src.a()) * dst.v;
		d.v = dst.v + (1.0f - dst.a()) * src.v;
		
		MLSimd32I4 comp = mlSimdLessThan(src.v * dst.a(), dst.v * src.a());
		
		if (comp[MLGlobal::ArgbRed]) d.r() = ds.r();
		if (comp[MLGlobal::ArgbGreen]) d.g() = ds.g();
		if (comp[MLGlobal::ArgbBlue]) d.b() = ds.b();
		
		dst = d;
	}
	
	static MLBlendOp::TileStates tileOpFunc(MLBlendOp::TileStates states)
	{
		return states;
	}
};

class BlendFunctionsLighten
{
public:
	static void blendFunc(MLArgb &dst, const MLArgb &src)
	{
		if (src.a() == 0) return;
		if (dst.a() == 0) { dst = src; return; }
		
		MLArgb d, ds;
		ds.v = src.v + (1.0f - src.a()) * dst.v;
		d.v = dst.v + (1.0f - dst.a()) * src.v;
		
		MLSimd32I4 comp = mlSimdGreaterThan(src.v * dst.a(), dst.v * src.a());
		
		if (comp[MLGlobal::ArgbRed]) d.r() = ds.r();
		if (comp[MLGlobal::ArgbGreen]) d.g() = ds.g();
		if (comp[MLGlobal::ArgbBlue]) d.b() = ds.b();
		
		dst = d;
	}
	
	static MLBlendOp::TileStates tileOpFunc(MLBlendOp::TileStates states)
	{
		return states;
	}
};

class BlendFunctionsColorDodge
{
public:
	static void blendFunc(MLArgb &dst, const MLArgb &src)
	{
		if (src.a() == 0) return;
		if (dst.a() == 0) { dst = src; return; }
		
		MLArgb d, d1, d2;
		d1.v = src.v * (1.0f - dst.a());
		d2.v = d1.v + (src.a() * dst.a()) + dst.v * (1.0f - src.a());
		d.v = (src.a() * dst.a()) * mlSimdMin(1.0f, dst.v * src.a() / (dst.a() * (src.a() - src.v)));
		
		MLSimd32I4 compS = mlSimdEqual(src.v, src.a());
		MLSimd32I4 compD = mlSimdEqual(dst.v, 0.0f);
		
		if (compS[MLGlobal::ArgbRed])
			d.r() = compD[MLGlobal::ArgbRed] ? d1.r() : d2.r();
		if (compS[MLGlobal::ArgbGreen])
			d.g() = compD[MLGlobal::ArgbGreen] ? d1.g() : d2.g();
		if (compS[MLGlobal::ArgbBlue])
			d.b() = compD[MLGlobal::ArgbBlue] ? d1.b() : d2.b();
		
		d.a() = src.a() + dst.a() - src.a() * dst.a();
		
		dst = d;
	}
	
	static MLBlendOp::TileStates tileOpFunc(MLBlendOp::TileStates states)
	{
		return states;
	}
};

class BlendFunctionsColorBurn
{
public:
	static void blendFunc(MLArgb &dst, const MLArgb &src)
	{
		if (src.a() == 0) return;
		if (dst.a() == 0) { dst = src; return; }
		
		MLArgb d, d1, d2;
		
		d2.v = dst.v * (1.0f - src.a());
		d1.v = (src.a() * dst.a()) + d2.v;
		d.v = d1.v + (src.a() * dst.a()) * mlSimdMin(1.0f, (dst.a() * dst.v) * src.a() / (dst.a() * src.v)) + src.v * (1.0f - dst.a());
		
		MLSimd32I4 compS = mlSimdEqual(src.v, 0.0f);
		MLSimd32I4 compD = mlSimdEqual(dst.v, dst.a());
		
		if (compS[MLGlobal::ArgbRed])
			d.r() = compD[MLGlobal::ArgbRed] ? d1.r() : d2.r();
		if (compS[MLGlobal::ArgbGreen])
			d.g() = compD[MLGlobal::ArgbGreen] ? d1.g() : d2.g();
		if (compS[MLGlobal::ArgbBlue])
			d.b() = compD[MLGlobal::ArgbBlue] ? d1.b() : d2.b();
		
		d.a() = src.a() + dst.a() - src.a() * dst.a();
		
		dst = d;
	}
	
	static MLBlendOp::TileStates tileOpFunc(MLBlendOp::TileStates states)
	{
		return states;
	}
};

class BlendFunctionsHardLight
{
public:
	static void blendFunc(MLArgb &dst, const MLArgb &src)
	{
		if (src.a() == 0) return;
		if (dst.a() == 0) { dst = src; return; }
		
		MLArgb d, d1, d2;
		
		d.v = src.v * (1.0f - dst.a()) + dst.v * (1.0f - src.a());
		d1.v = 2.0f * src.v;
		d2.v = (src.a() * dst.a()) - 2.0f * (dst.a() - dst.v) * (src.a() - src.v);
		
		MLSimd32I4 comp = mlSimdLessThanOrEqual(2.0f * src.v, src.a());
		
		d.r() += comp[MLGlobal::ArgbRed] ? d1.r() : d2.r();
		d.g() += comp[MLGlobal::ArgbGreen] ? d1.g() : d2.g();
		d.b() += comp[MLGlobal::ArgbBlue] ? d1.b() : d2.b();
		d.a() += src.a() * dst.a();
		
		dst = d;
	}
	
	static MLBlendOp::TileStates tileOpFunc(MLBlendOp::TileStates states)
	{
		return states;
	}
};

class BlendFunctionsSoftLight
{
public:
	static void blendFunc(MLArgb &dst, const MLArgb &src)
	{
		MLArgb m, f, d0, d1, d2, d3;
		m.v = dst.v / dst.a();
		f.v = 2.0f * src.v - src.a();
		d0.v = src.v * (1.0f - dst.a()) + dst.v;
		d1.v = dst.v * f.v * (1 - m.v) + d0.v;
		d2.v = dst.a() * f.v * (4.0f * m.v * (4.0f * m.v + 1.0f) * (m.v - 1.0f) + 7.0f * m.v) + d0.v;
		d3.v = dst.a() * f.v * (mlSimdSqrt(m.v) - m.v) + d0.v;
		
		MLSimd32I4 compS = mlSimdLessThanOrEqual(2.0f * src.v, src.a());
		MLSimd32I4 compD = mlSimdLessThanOrEqual(4.0f * dst.v, dst.a());
		
		dst.a() = d0.a();
		
		if (compS[MLGlobal::ArgbRed])
			dst.r() = d1.r();
		else
			dst.r() = compD[MLGlobal::ArgbRed] ? d2.r() : d3.r();
		
		if (compS[MLGlobal::ArgbGreen])
			dst.g() = d1.g();
		else
			dst.g() = compD[MLGlobal::ArgbGreen] ? d2.g() : d3.b();
		
		if (compS[MLGlobal::ArgbBlue])
			dst.b() = d1.b();
		else
			dst.b() = compD[MLGlobal::ArgbBlue] ? d2.b() : d3.b();
	}
	
	static MLBlendOp::TileStates tileOpFunc(MLBlendOp::TileStates states)
	{
		return states;
	}
};

class BlendFunctionsDifference
{
public:
	static void blendFunc(MLArgb &dst, const MLArgb &src)
	{
		MLArgb d;
		d.v = src.v + dst.v - 2.0f * mlSimdMin(src.v * dst.a(), dst.v * src.a());
		d.a() += src.a() * dst.a();
		dst = d;
	}
	
	static MLBlendOp::TileStates tileOpFunc(MLBlendOp::TileStates states)
	{
		return states;
	}
};

class BlendFunctionsExclusion
{
public:
	static void blendFunc(MLArgb &dst, const MLArgb &src)
	{
		MLArgb d;
		d.v = (src.v * dst.a() + dst.v * src.a() - (2.0f * src.a() * dst.a())) + src.v * (1.0f - dst.a()) + dst.v * (1.0f - src.a());
		d.a() += src.a() * dst.a();
		dst = d;
	}
	
	static MLBlendOp::TileStates tileOpFunc(MLBlendOp::TileStates states)
	{
		return states;
	}
};

MLBlendOpDictionary::MLBlendOpDictionary()
{
	_blendOps[MLGlobal::BlendModeClear] = new MLTemplateBlendOp<BlendFunctionsClear>;
	_blendOps[MLGlobal::BlendModeSource] = new MLTemplateBlendOp<BlendFunctionsSource>;
	_blendOps[MLGlobal::BlendModeDestination] = new MLTemplateBlendOp<BlendFunctionsDestination>;
	_blendOps[MLGlobal::BlendModeSourceOver] = new MLTemplateBlendOp<BlendFunctionsSourceOver>;
	_blendOps[MLGlobal::BlendModeDestinationOver] = new MLTemplateBlendOp<BlendFunctionsDestinationOver>;
	_blendOps[MLGlobal::BlendModeSourceIn] = new MLTemplateBlendOp<BlendFunctionsSourceIn>;
	_blendOps[MLGlobal::BlendModeDestinationIn] = new MLTemplateBlendOp<BlendFunctionsDestinationIn>;
	_blendOps[MLGlobal::BlendModeSourceOut] = new MLTemplateBlendOp<BlendFunctionsSourceOut>;
	_blendOps[MLGlobal::BlendModeDestinationOut] = new MLTemplateBlendOp<BlendFunctionsDestinationOut>;
	_blendOps[MLGlobal::BlendModeSourceAtop] = new MLTemplateBlendOp<BlendFunctionsSourceAtop>;
	_blendOps[MLGlobal::BlendModeDestinationAtop] = new MLTemplateBlendOp<BlendFunctionsDestinationAtop>;
	_blendOps[MLGlobal::BlendModeXor] = new MLTemplateBlendOp<BlendFunctionsXor>;
	
	_blendOps[MLGlobal::BlendModeNormal] = _blendOps.value(MLGlobal::BlendModeSourceOver);
	_blendOps[MLGlobal::BlendModePlus] = new MLTemplateBlendOp<BlendFunctionsPlus>;
	_blendOps[MLGlobal::BlendModeMultiply] = new MLTemplateBlendOp<BlendFunctionsMultiply>;
	_blendOps[MLGlobal::BlendModeScreen] = new MLTemplateBlendOp<BlendFunctionsScreen>;
	_blendOps[MLGlobal::BlendModeOverlay] = new MLTemplateBlendOp<BlendFunctionsOverlay>;
	_blendOps[MLGlobal::BlendModeDarken] = new MLTemplateBlendOp<BlendFunctionsDarken>;
	_blendOps[MLGlobal::BlendModeLighten] = new MLTemplateBlendOp<BlendFunctionsLighten>;
	_blendOps[MLGlobal::BlendModeColorDodge] = new MLTemplateBlendOp<BlendFunctionsColorDodge>;
	_blendOps[MLGlobal::BlendModeColorBurn] = new MLTemplateBlendOp<BlendFunctionsColorBurn>;
	_blendOps[MLGlobal::BlendModeHardLight] = new MLTemplateBlendOp<BlendFunctionsHardLight>;
	_blendOps[MLGlobal::BlendModeSoftLight] = new MLTemplateBlendOp<BlendFunctionsSoftLight>;
	_blendOps[MLGlobal::BlendModeDifference] = new MLTemplateBlendOp<BlendFunctionsDifference>;
	_blendOps[MLGlobal::BlendModeExclusion] = new MLTemplateBlendOp<BlendFunctionsExclusion>;
}

MLBlendOpDictionary BlendOpDictionary;

MLBlendOpDictionary *mlBlendOpDictionary()
{
	return &BlendOpDictionary;
}

