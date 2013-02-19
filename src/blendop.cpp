#include "blendmode.h"
#include "blendop.h"

// Pixel blend mode based on SVG compositing specification

namespace Malachite
{

static const PixelVec pixelVecZero(0.f);
static const PixelVec pixelVecOne(1.f);

inline static Pixel compose(const Pixel &dst, const Pixel &src, const Pixel &f_sa_da, float x, float y, float z)
{
	Pixel fx = f_sa_da;
	fx.ra() = x;
	
	float da = dst.a();
	float sa = src.a();
	
	Pixel r;
	r.rv() = fx.v() + PixelVec(y * (1.f - da)) * src.v() + PixelVec(z * (1.f - sa)) * dst.v();
	return r;
}

class BlendFunctionsClear
{
public:
	static void blend(Pixel &dst, const Pixel &src)
	{
		Q_UNUSED(src);
		dst = Pixel(0);
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
	static void blend(Pixel &dst, const Pixel &src)
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
	static void blend(Pixel &dst, const Pixel &src)
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
	static void blend(Pixel &dst, const Pixel &src)
	{
		dst.rv() = src.v() + (pixelVecOne - src.aV()) * dst.v();
		//dst = src + (1.0f - src.a) * dst;
	}
	
	static BlendOp::TileCombination tileRequirement(BlendOp::TileCombination states)
	{
		return states;
	}
};

class BlendFunctionsDestinationOver
{
public:
	static void blend(Pixel &dst, const Pixel &src)
	{
		dst.rv() = dst.v() + (pixelVecOne - dst.aV()) * src.v();
		//dst = dst + (1.0f - dst.a) * src;
	}
	
	static BlendOp::TileCombination tileRequirement(BlendOp::TileCombination states)
	{
		return states;
	}
};

class BlendFunctionsSourceIn
{
public:
	static void blend(Pixel &dst, const Pixel &src)
	{
		dst.rv() = dst.aV() * src.v();
		//dst = dst.a * src;
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
	static void blend(Pixel &dst, const Pixel &src)
	{
		dst.rv() = src.aV() * dst.v();
		//dst = src.a * dst;
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
	static void blend(Pixel &dst, const Pixel &src)
	{
		dst.rv() = (pixelVecOne - dst.aV()) * src.v();
		//dst = (1.0f - dst.a) * src;
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
	static void blend(Pixel &dst, const Pixel &src)
	{
		dst.rv() = (pixelVecOne - src.aV()) * dst.v();
		//dst = (1.0f - src.a) * dst;
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
	static void blend(Pixel &dst, const Pixel &src)
	{
		dst.rv() = dst.aV() * src.v() + (pixelVecOne - src.aV()) + dst.v();
		//dst = dst.a * src + (1.0f - src.a) * dst;
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
	static void blend(Pixel &dst, const Pixel &src)
	{
		dst.rv() = src.aV() * dst.v() + (pixelVecOne - dst.v()) * src.v();
		//dst = src.a * dst + (1.0f - dst.a) * src;
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
	static void blend(Pixel &dst, const Pixel &src)
	{
		dst.rv() = (pixelVecOne - dst.aV()) * src.v() + (pixelVecOne - src.aV()) * dst.v();
		//dst = (1.0f - dst.a) * src + (1.0f - src.a) * dst;
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
	static void blend(Pixel &dst, const Pixel &src)
	{
		dst.rv() = (dst.v() + src.v()).bound(PixelVec(0), PixelVec(1));
		//dst = vecBound(0, dst + src , 1);
	}
	
	static BlendOp::TileCombination tileRequirement(BlendOp::TileCombination states) { return states; }
};

class BlendFunctionsMultiply
{
public:
	static void blend(Pixel &dst, const Pixel &src)
	{
		dst.rv() = src.v() * dst.v() + src.v() * (PixelVec(1) - dst.aV()) + dst.v() * (PixelVec(1) - src.aV());
		//dst = src * dst + src * (1.0f - dst.a) + dst * (1.0f - src.a);
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
	static void blend(Pixel &dst, const Pixel &src)
	{
		dst.rv() = src.v() * dst.v() - src.v() * dst.v();
		//dst = src + dst - src * dst;
	}
	
	static BlendOp::TileCombination tileRequirement(BlendOp::TileCombination states) { return states; }
};

class BlendFunctionsOverlay
{
public:
	static void blend(Pixel &dst, const Pixel &src)
	{
		if (src.a() == 0) return;
		if (dst.a() == 0) { dst = src; return; }
		
		Pixel f1, f2, f;
		f1.rv() = PixelVec(2) * src.v() * dst.v();
		f2.rv() = PixelVec(src.a() * dst.a()) - PixelVec(2) * (dst.aV() - dst.v()) * (src.aV() - src.v());
		f.rv() == PixelVec::choose( PixelVec::lessThanEqual(dst.v(), PixelVec(dst.a() * 0.5f)), f1.v(), f2.v() );
		
		dst = compose(dst, src, f, 1, 1, 1);
	}
	
	static BlendOp::TileCombination tileRequirement(BlendOp::TileCombination states) { return states; }
};

class BlendFunctionsDarken
{
public:
	static void blend(Pixel &dst, const Pixel &src)
	{
		if (src.a() == 0) return;
		if (dst.a() == 0) { dst = src; return; }
		
		Pixel f;
		f.rv() = PixelVec::minimum(src.v() * dst.aV(), dst.v() * src.aV());
		dst = compose(dst, src, f, 1, 1, 1);
	}
	
	static BlendOp::TileCombination tileRequirement(BlendOp::TileCombination states) { return states; }
};

class BlendFunctionsLighten
{
public:
	static void blend(Pixel &dst, const Pixel &src)
	{
		if (src.a() == 0) return;
		if (dst.a() == 0) { dst = src; return; }
		
		Pixel f;
		f.rv() = PixelVec::maximum(src.v() * dst.aV(), dst.v() * src.aV());
		dst = compose(dst, src, f, 1, 1, 1);
	}
	
	static BlendOp::TileCombination tileRequirement(BlendOp::TileCombination states) { return states; }
};

class BlendFunctionsColorDodge
{
public:
	static void blend(Pixel &dst, const Pixel &src)
	{
		if (src.a() == 0) return;
		if (dst.a() == 0) { dst = src; return; }
		
		Pixel f;
		
		f.rv() = PixelVec::minimum( PixelVec(dst.a() * src.a()), dst.v() * (src.a() * src.a()) / (src.aV() - src.v()) );
		f.rv() = PixelVec::choose(PixelVec::equal(src.v(), src.aV()), PixelVec(1), f.v());
		
		dst = compose(dst, src, f, 1, 1, 1);
	}
	
	static BlendOp::TileCombination tileRequirement(BlendOp::TileCombination states) { return states; }
};

class BlendFunctionsColorBurn
{
public:
	static void blend(Pixel &dst, const Pixel &src)
	{
		if (src.a() == 0) return;
		if (dst.a() == 0) { dst = src; return; }
		
		Pixel f;
		PixelVec sada(src.a() * dst.a());
		f.rv() = sada - PixelVec::minimum( sada, (dst.aV() - dst.v()) * (src.a() * src.a()) / src.v() );
		f.rv() = PixelVec::choose( PixelVec::equal(src.v(), PixelVec(0)), PixelVec(0), f.v());
		
		dst = compose(dst, src, f, 1, 1, 1);
	}
	
	static BlendOp::TileCombination tileRequirement(BlendOp::TileCombination states) { return states; }
};

class BlendFunctionsHardLight
{
public:
	static void blend(Pixel &dst, const Pixel &src)
	{
		if (src.a() == 0) return;
		if (dst.a() == 0) { dst = src; return; }
		
		Pixel f1, f2, f;
		f1.rv() = 2.f * src.v() * dst.v();
		f2.rv() = PixelVec(src.a() * dst.a()) - 2.f * (dst.aV() - dst.v()) * (src.aV() - src.v());
		f.rv() = PixelVec::choose( PixelVec::lessThan(src.v(), PixelVec(src.a() * 0.5)), f1.v(), f2.v() );
		
		dst = compose(dst, src, f, 1, 1, 1);
	}
	
	static BlendOp::TileCombination tileRequirement(BlendOp::TileCombination states) { return states; }
};

class BlendFunctionsSoftLight
{
public:
	static void blend(Pixel &dst, const Pixel &src)
	{
		if (src.a() == 0) return;
		if (dst.a() == 0) { dst = src; return; }
		
		Pixel dc, f1, f2, f3, f;
		
		dc.rv() = dst.v() / dst.aV();
		
		f1.rv() = dc.v() * ((dst.a() * src.a()) - (src.aV() - 2.f * src.v() * (dst.aV() - dst.v())));
		f2.rv() = dst.v() * src.aV() + ( 2.f * src.v() - src.aV() );
		f3.rv() = dst.v() * (2.f * src.v() - PixelVec(1)) * ( sseSqrt(dc.v()) - dc.v());
		
		f.rv() = PixelVec::choose( PixelVec::lessThan(src.v(), PixelVec(0.5f * src.a())),
		                           f1.v(),
		                           PixelVec::choose ( PixelVec::lessThan(dst.v(), PixelVec(0.25f * dst.a())), f2.v(), f3.v() ));
		
		dst = compose(dst, src, f, 1, 1, 1);
	}
	
	static BlendOp::TileCombination tileRequirement(BlendOp::TileCombination states) { return states; }
};

class BlendFunctionsDifference
{
public:
	static void blend(Pixel &dst, const Pixel &src)
	{
		if (src.a() == 0) return;
		if (dst.a() == 0) { dst = src; return; }
		
		Pixel d;
		d.rv() = src.v() + dst.v() - 2.f * PixelVec::minimum(src.v() * dst.aV(), dst.v() * src.aV());
		d.ra() += src.a() * dst.a();
		dst = d;
	}
	
	static BlendOp::TileCombination tileRequirement(BlendOp::TileCombination states) { return states; }
};

class BlendFunctionsExclusion
{
public:
	static void blend(Pixel &dst, const Pixel &src)
	{
		if (src.a() == 0) return;
		if (dst.a() == 0) { dst = src; return; }
		
		Pixel f;
		f.rv() = src.v() * dst.aV() + dst.v() * src.aV() - 2.f * src.v() * dst.v();
		dst = compose(dst, src, f, 1, 1, 1);
	}
	
	static BlendOp::TileCombination tileRequirement(BlendOp::TileCombination states) { return states; }
};


BlendOpDictionary::BlendOpDictionary()
{
	_blendOps[BlendMode::Clear] = new TemplateBlendOp<BlendFunctionsClear>;
	_blendOps[BlendMode::Source] = new TemplateBlendOp<BlendFunctionsSource>;
	_blendOps[BlendMode::Destination] = new TemplateBlendOp<BlendFunctionsDestination>;
	_blendOps[BlendMode::SourceOver] = new TemplateBlendOp<BlendFunctionsSourceOver>;
	_blendOps[BlendMode::DestinationOver] = new TemplateBlendOp<BlendFunctionsDestinationOver>;
	_blendOps[BlendMode::SourceIn] = new TemplateBlendOp<BlendFunctionsSourceIn>;
	_blendOps[BlendMode::DestinationIn] = new TemplateBlendOp<BlendFunctionsDestinationIn>;
	_blendOps[BlendMode::SourceOut] = new TemplateBlendOp<BlendFunctionsSourceOut>;
	_blendOps[BlendMode::DestinationOut] = new TemplateBlendOp<BlendFunctionsDestinationOut>;
	_blendOps[BlendMode::SourceAtop] = new TemplateBlendOp<BlendFunctionsSourceAtop>;
	_blendOps[BlendMode::DestinationAtop] = new TemplateBlendOp<BlendFunctionsDestinationAtop>;
	_blendOps[BlendMode::Xor] = new TemplateBlendOp<BlendFunctionsXor>;
	
	_blendOps[BlendMode::Normal] = _blendOps[BlendMode::SourceOver];
	_blendOps[BlendMode::Plus] = new TemplateBlendOp<BlendFunctionsPlus>;
	_blendOps[BlendMode::Multiply] = new TemplateBlendOp<BlendFunctionsMultiply>;
	_blendOps[BlendMode::Screen] = new TemplateBlendOp<BlendFunctionsScreen>;
	_blendOps[BlendMode::Overlay] = new TemplateBlendOp<BlendFunctionsOverlay>;
	_blendOps[BlendMode::Darken] = new TemplateBlendOp<BlendFunctionsDarken>;
	_blendOps[BlendMode::Lighten] = new TemplateBlendOp<BlendFunctionsLighten>;
	_blendOps[BlendMode::ColorDodge] = new TemplateBlendOp<BlendFunctionsColorDodge>;
	_blendOps[BlendMode::ColorBurn] = new TemplateBlendOp<BlendFunctionsColorBurn>;
	_blendOps[BlendMode::HardLight] = new TemplateBlendOp<BlendFunctionsHardLight>;
	_blendOps[BlendMode::SoftLight] = new TemplateBlendOp<BlendFunctionsSoftLight>;
	_blendOps[BlendMode::Difference] = new TemplateBlendOp<BlendFunctionsDifference>;
	_blendOps[BlendMode::Exclusion] = new TemplateBlendOp<BlendFunctionsExclusion>;
}

BlendOpDictionary _BlendOpDictionary;

BlendOpDictionary *blendOpDictionary()
{
	return &_BlendOpDictionary;
}

}

