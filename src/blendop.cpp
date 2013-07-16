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

struct BlendTraitsClear
{
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

struct BlendTraitsSource
{
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

struct BlendTraitsDestination
{
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

struct BlendTraitsSourceOver
{
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

struct BlendTraitsDestinationOver
{
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

struct BlendTraitsSourceIn
{
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

struct BlendTraitsDestinationIn
{
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

struct BlendTraitsSourceOut
{
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

struct BlendTraitsDestinationOut
{
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

struct BlendTraitsSourceAtop
{
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

struct BlendTraitsDestinationAtop
{
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

struct BlendTraitsXor
{
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

struct BlendTraitsPlus : public BlendTraitsSourceOver
{
	static void blend(Pixel &dst, const Pixel &src)
	{
		dst.rv() = (dst.v() + src.v()).bound(0.f, 1.f);
		//dst = vecBound(0, dst + src , 1);
	}
};

struct BlendTraitsMultiply
{
	static void blend(Pixel &dst, const Pixel &src)
	{
		dst.rv() = src.v() * dst.v() + src.v() * (1.f - dst.aV()) + dst.v() * (1.f - src.aV());
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

struct BlendTraitsScreen : public BlendTraitsSourceOver
{
	static void blend(Pixel &dst, const Pixel &src)
	{
		dst.rv() = src.v() + dst.v() - src.v() * dst.v();
		//dst = src + dst - src * dst;
	}
};

struct BlendTraitsOverlay : public BlendTraitsSourceOver
{
	static void blend(Pixel &dst, const Pixel &src)
	{
		Pixel c1, c2;
		c1.rv() = src.v() * ( 2.f * dst.v() + ( 1.f - dst.a() ) ) + dst.v() * ( 1.f - src.a() );
		c2.rv() = src.v() * ( 1.f + dst.a()) + dst.v() * ( 1.f + src.a() ) - 2.f * dst.v() * src.v() - dst.a() * src.a();
		dst.rv() = PixelVec::choose( PixelVec::lessThanEqual( dst.v(), dst.a() * 0.5f ), c1.v(), c2.v() );
	}
};

struct BlendTraitsDarken : public BlendTraitsSourceOver
{
	static void blend(Pixel &dst, const Pixel &src)
	{
		Pixel srcOver, dstOver;
		
		srcOver.rv() = src.v() + ( 1.f - src.a() ) * dst.v();
		dstOver.rv() = dst.v() + ( 1.f - dst.a() ) * src.v();
		
		dst.rv() = PixelVec::choose( PixelVec::lessThan( src.v() * dst.aV(), dst.v() * src.aV() ), srcOver.v(), dstOver.v() );
	}
};

struct BlendTraitsLighten : public BlendTraitsSourceOver
{
	static void blend(Pixel &dst, const Pixel &src)
	{
		Pixel srcOver, dstOver;
		
		srcOver.rv() = src.v() + ( 1.f - src.a() ) * dst.v();
		dstOver.rv() = dst.v() + ( 1.f - dst.a() ) * src.v();
		
		dst.rv() = PixelVec::choose( PixelVec::greaterThan( src.v() * dst.aV(), dst.v() * src.aV() ), srcOver.v(), dstOver.v() );
	}
};

struct BlendTraitsColorDodge : public BlendTraitsSourceOver
{
	static void blend(Pixel &dst, const Pixel &src)
	{
		if (dst.a() == 0.f)
		{
			dst = src;
			return;
		}
		
		Pixel c1, c2, c3;
		c1.rv() = src.v() * (1.f - dst.a());
		c2.rv() = c1.v() + src.a() * dst.a() + dst.v() * ( 1.f - src.a() );
		c3.rv() = src.a() * dst.a() * PixelVec::minimum( 1.f, dst.v() * src.a() / ( dst.a() * ( src.a() - src.v() ) ) )
				+ c1.v()
				+ dst.v() * (1.f - src.a());
		
		Pixel c4;
		c4.rv() = PixelVec::choose( PixelVec::equal( dst.v(), 0.f ), c1.v(), c2.v() );
		dst.rv() = PixelVec::choose( PixelVec::equal( src.v(), src.a() ), c4.v(), c3.v() );
	}
};

struct BlendTraitsColorBurn : public BlendTraitsSourceOver
{
	static void blend(Pixel &dst, const Pixel &src)
	{
		if (dst.a() == 0.f)
		{
			dst = src;
			return;
		}
		
		Pixel c1, c2, c3;
		c2.rv() = dst.v() * (1.f - src.a());
		c1.rv() = src.a() * dst.a() + c2.v();
		c3.rv() = src.a() * dst.a() * ( 1.f - PixelVec::minimum( 1.f, ( dst.a() - dst.v() ) * src.a() / ( dst.a() * src.v() ) ) )
				+ src.v() * (1.f - dst.a())
				+ c2.v();
		
		Pixel c4;
		c4.rv() = PixelVec::choose( PixelVec::equal( dst.v(), dst.a() ), c1.v(), c2.v() );
		dst.rv() = PixelVec::choose( PixelVec::equal( src.v(), 0.f ), c4.v(), c3.v() );
	}
};

struct BlendTraitsHardLight : public BlendTraitsSourceOver
{
	static void blend(Pixel &dst, const Pixel &src)
	{
		Pixel c1, c2;
		
		c1.rv() = 2.f * src.v() * dst.v() + src.v() * (1.f - dst.a()) + dst.v() * (1.f - src.a());
		c2.rv() = src.v() * (1.f + dst.a()) + dst.v() * (1.f + src.a()) - src.a() * dst.a() - 2 * src.v() * dst.v();
		
		dst.rv() = PixelVec::choose( PixelVec::lessThanEqual(src.v(), src.a() * 0.5f), c1.v(), c2.v() );
	}
};

struct BlendTraitsSoftLight : public BlendTraitsSourceOver
{
	static void blend(Pixel &dst, const Pixel &src)
	{
		if (dst.a() == 0.f)
		{
			dst = src;
			return;
		}
		
		Pixel m;
		m.rv() = dst.v() / dst.a();
		
		Pixel c1, c2, c3;
		c1.rv() = dst.v() * ( src.a() + ( 2.f * src.v() - src.a() ) * (1.f - m.v()) )
				+ src.v() * ( 1.f - dst.a() )
				+ dst.v() * ( 1.f - src.a() );
		c2.rv() = dst.a() * ( 2.f * src.v() - src.a() ) * ( 16.f * m.v() * m.v() *m.v() - 12.f * m.v() *m.v() - 3.f * m.v() )
				+ src.v() * ( 1.f - dst.a() )
				+ dst.v();
		c3.rv() = dst.a() * ( 2.f * src.v() - src.a() ) * ( sseSqrt( m.v() ) - m.v() )
				+ src.v() * ( 1.f - dst.a() )
				+ dst.v();
		
		Pixel c4;
		c4.rv() = PixelVec::choose( PixelVec::lessThanEqual( dst.v(), 0.25f * dst.a() ), c2.v(), c3.v() );
		dst.rv() = PixelVec::choose( PixelVec::lessThanEqual( src.v(), 0.5f * src.a() ), c1.v(), c4.v() );
	}
};

struct BlendTraitsDifference : public BlendTraitsSourceOver
{
	static void blend(Pixel &dst, const Pixel &src)
	{
		Pixel d;
		d.rv() = src.v() + dst.v() - 2.f * PixelVec::minimum(src.v() * dst.aV(), dst.v() * src.aV());
		d.ra() += src.a() * dst.a();
		dst = d;
	}
};

struct BlendTraitsExclusion : public BlendTraitsSourceOver
{
	static void blend(Pixel &dst, const Pixel &src)
	{
		Pixel d;
		d.rv() = src.v() + dst.v() - 2.f * src.v() * dst.v();
		d.ra() += src.a() * dst.a();
		dst = d;
	}
};


BlendOpDictionary::BlendOpDictionary()
{
	_blendOps[BlendMode::Clear] = new TemplateBlendOp<BlendTraitsClear>;
	_blendOps[BlendMode::Source] = new TemplateBlendOp<BlendTraitsSource>;
	_blendOps[BlendMode::Destination] = new TemplateBlendOp<BlendTraitsDestination>;
	_blendOps[BlendMode::SourceOver] = new TemplateBlendOp<BlendTraitsSourceOver>;
	_blendOps[BlendMode::DestinationOver] = new TemplateBlendOp<BlendTraitsDestinationOver>;
	_blendOps[BlendMode::SourceIn] = new TemplateBlendOp<BlendTraitsSourceIn>;
	_blendOps[BlendMode::DestinationIn] = new TemplateBlendOp<BlendTraitsDestinationIn>;
	_blendOps[BlendMode::SourceOut] = new TemplateBlendOp<BlendTraitsSourceOut>;
	_blendOps[BlendMode::DestinationOut] = new TemplateBlendOp<BlendTraitsDestinationOut>;
	_blendOps[BlendMode::SourceAtop] = new TemplateBlendOp<BlendTraitsSourceAtop>;
	_blendOps[BlendMode::DestinationAtop] = new TemplateBlendOp<BlendTraitsDestinationAtop>;
	_blendOps[BlendMode::Xor] = new TemplateBlendOp<BlendTraitsXor>;
	
	_blendOps[BlendMode::Normal] = _blendOps[BlendMode::SourceOver];
	_blendOps[BlendMode::Plus] = new TemplateBlendOp<BlendTraitsPlus>;
	_blendOps[BlendMode::Multiply] = new TemplateBlendOp<BlendTraitsMultiply>;
	_blendOps[BlendMode::Screen] = new TemplateBlendOp<BlendTraitsScreen>;
	_blendOps[BlendMode::Overlay] = new TemplateBlendOp<BlendTraitsOverlay>;
	_blendOps[BlendMode::Darken] = new TemplateBlendOp<BlendTraitsDarken>;
	_blendOps[BlendMode::Lighten] = new TemplateBlendOp<BlendTraitsLighten>;
	_blendOps[BlendMode::ColorDodge] = new TemplateBlendOp<BlendTraitsColorDodge>;
	_blendOps[BlendMode::ColorBurn] = new TemplateBlendOp<BlendTraitsColorBurn>;
	_blendOps[BlendMode::HardLight] = new TemplateBlendOp<BlendTraitsHardLight>;
	_blendOps[BlendMode::SoftLight] = new TemplateBlendOp<BlendTraitsSoftLight>;
	_blendOps[BlendMode::Difference] = new TemplateBlendOp<BlendTraitsDifference>;
	_blendOps[BlendMode::Exclusion] = new TemplateBlendOp<BlendTraitsExclusion>;
}

BlendOpDictionary _BlendOpDictionary;

BlendOpDictionary *blendOpDictionary()
{
	return &_BlendOpDictionary;
}

}

