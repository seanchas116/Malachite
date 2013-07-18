#include "blendmode.h"
#include "blendop.h"

// Pixel blend mode based on SVG compositing specification

namespace Malachite
{

static const PixelVec pixelVecZero(0.f);
static const PixelVec pixelVecOne(1.f);

struct BlendTraitsClear
{
	static Pixel blend(const Pixel &dst, const Pixel &src)
	{
		Q_UNUSED(src);
		return Pixel(0);
	}
	
	static BlendOp::TileCombination tileRequirement(BlendOp::TileCombination states)
	{
		Q_UNUSED(states);
		return BlendOp::NoTile;
	}
};

struct BlendTraitsSource
{
	static Pixel blend(const Pixel &dst, const Pixel &src)
	{
		Q_UNUSED(dst);
		return src;
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
	static Pixel blend(const Pixel &dst, const Pixel &src)
	{
		Q_UNUSED(src);
		return dst;
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
	static Pixel blend(const Pixel &dst, const Pixel &src)
	{
		return src.v() + (pixelVecOne - src.aV()) * dst.v();
		//dst = src + (1.0f - src.a) * dst;
	}
	
	static BlendOp::TileCombination tileRequirement(BlendOp::TileCombination states)
	{
		return states;
	}
};

struct BlendTraitsDestinationOver
{
	static Pixel blend(const Pixel &dst, const Pixel &src)
	{
		return dst.v() + (pixelVecOne - dst.aV()) * src.v();
		//dst = dst + (1.0f - dst.a) * src;
	}
	
	static BlendOp::TileCombination tileRequirement(BlendOp::TileCombination states)
	{
		return states;
	}
};

struct BlendTraitsSourceIn
{
	static Pixel blend(const Pixel &dst, const Pixel &src)
	{
		return dst.aV() * src.v();
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
	static Pixel blend(const Pixel &dst, const Pixel &src)
	{
		return src.aV() * dst.v();
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
	static Pixel blend(const Pixel &dst, const Pixel &src)
	{
		return (pixelVecOne - dst.aV()) * src.v();
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
	static Pixel blend(const Pixel &dst, const Pixel &src)
	{
		return (pixelVecOne - src.aV()) * dst.v();
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
	static Pixel blend(const Pixel &dst, const Pixel &src)
	{
		return dst.aV() * src.v() + (pixelVecOne - src.aV()) + dst.v();
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
	static Pixel blend(const Pixel &dst, const Pixel &src)
	{
		return src.aV() * dst.v() + (pixelVecOne - dst.v()) * src.v();
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
	static Pixel blend(const Pixel &dst, const Pixel &src)
	{
		return (pixelVecOne - dst.aV()) * src.v() + (pixelVecOne - src.aV()) * dst.v();
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
	static Pixel blend(const Pixel &dst, const Pixel &src)
	{
		return (dst.v() + src.v()).bound(0.f, 1.f);
		//dst = vecBound(0, dst + src , 1);
	}
};

struct BlendTraitsMultiply : public BlendTraitsSourceOver
{
	static Pixel blend(const Pixel &dst, const Pixel &src)
	{
		return src.v() * dst.v() + src.v() * (1.f - dst.aV()) + dst.v() * (1.f - src.aV());
		//dst = src * dst + src * (1.0f - dst.a) + dst * (1.0f - src.a);
	}
};

struct BlendTraitsScreen : public BlendTraitsSourceOver
{
	static Pixel blend(const Pixel &dst, const Pixel &src)
	{
		return src.v() + dst.v() - src.v() * dst.v();
		//dst = src + dst - src * dst;
	}
};

struct BlendTraitsOverlay : public BlendTraitsSourceOver
{
	static Pixel blend(const Pixel &dst, const Pixel &src)
	{
		Pixel c1 = src.v() * ( 2.f * dst.v() + ( 1.f - dst.a() ) ) + dst.v() * ( 1.f - src.a() );
		Pixel c2 = src.v() * ( 1.f + dst.a()) + dst.v() * ( 1.f + src.a() ) - 2.f * dst.v() * src.v() - dst.a() * src.a();
		return PixelVec::choose( PixelVec::lessThanEqual( dst.v(), dst.a() * 0.5f ), c1.v(), c2.v() );
	}
};

struct BlendTraitsDarken : public BlendTraitsSourceOver
{
	static Pixel blend(const Pixel &dst, const Pixel &src)
	{
		auto srcOver = BlendTraitsSourceOver::blend(dst, src);
		auto dstOver = BlendTraitsDestinationOver::blend(dst, src);
		return PixelVec::choose( PixelVec::lessThan( src.v() * dst.aV(), dst.v() * src.aV() ), srcOver.v(), dstOver.v() );
	}
};

struct BlendTraitsLighten : public BlendTraitsSourceOver
{
	static Pixel blend(const Pixel &dst, const Pixel &src)
	{
		auto srcOver = BlendTraitsSourceOver::blend(dst, src);
		auto dstOver = BlendTraitsDestinationOver::blend(dst, src);
		return PixelVec::choose( PixelVec::greaterThan( src.v() * dst.aV(), dst.v() * src.aV() ), srcOver.v(), dstOver.v() );
	}
};

struct BlendTraitsColorDodge : public BlendTraitsSourceOver
{
	static Pixel blend(const Pixel &dst, const Pixel &src)
	{
		if (dst.a() == 0.f)
			return src;
		
		Pixel c1 = src.v() * (1.f - dst.a());
		Pixel c2 = c1.v() + src.a() * dst.a() + dst.v() * ( 1.f - src.a() );
		Pixel c3 = src.a() * dst.a() * PixelVec::minimum( 1.f, dst.v() * src.a() / ( dst.a() * ( src.a() - src.v() ) ) )
				+ c1.v()
				+ dst.v() * (1.f - src.a());
		
		Pixel c4 = PixelVec::choose( PixelVec::equal( dst.v(), 0.f ), c1.v(), c2.v() );
		return PixelVec::choose( PixelVec::equal( src.v(), src.a() ), c4.v(), c3.v() );
	}
};

struct BlendTraitsColorBurn : public BlendTraitsSourceOver
{
	static Pixel blend(const Pixel &dst, const Pixel &src)
	{
		if (dst.a() == 0.f)
			return src;
		
		Pixel c2 = dst.v() * (1.f - src.a());
		Pixel c1 = src.a() * dst.a() + c2.v();
		Pixel c3 = src.a() * dst.a() * ( 1.f - PixelVec::minimum( 1.f, ( dst.a() - dst.v() ) * src.a() / ( dst.a() * src.v() ) ) )
				+ src.v() * (1.f - dst.a())
				+ c2.v();
		
		Pixel c4 = PixelVec::choose( PixelVec::equal( dst.v(), dst.a() ), c1.v(), c2.v() );
		return PixelVec::choose( PixelVec::equal( src.v(), 0.f ), c4.v(), c3.v() );
	}
};

struct BlendTraitsHardLight : public BlendTraitsSourceOver
{
	static Pixel blend(const Pixel &dst, const Pixel &src)
	{
		Pixel c1 = 2.f * src.v() * dst.v() + src.v() * (1.f - dst.a()) + dst.v() * (1.f - src.a());
		Pixel c2 = src.v() * (1.f + dst.a()) + dst.v() * (1.f + src.a()) - src.a() * dst.a() - 2 * src.v() * dst.v();
		
		return PixelVec::choose( PixelVec::lessThanEqual(src.v(), src.a() * 0.5f), c1.v(), c2.v() );
	}
};

struct BlendTraitsSoftLight : public BlendTraitsSourceOver
{
	static Pixel blend(const Pixel &dst, const Pixel &src)
	{
		if (dst.a() == 0.f)
			return src;
		
		Pixel m = dst.v() / dst.a();
		
		Pixel c1 = dst.v() * ( src.a() + ( 2.f * src.v() - src.a() ) * (1.f - m.v()) )
				+ src.v() * ( 1.f - dst.a() )
				+ dst.v() * ( 1.f - src.a() );
		Pixel c2 = dst.a() * ( 2.f * src.v() - src.a() ) * ( 16.f * m.v() * m.v() *m.v() - 12.f * m.v() *m.v() - 3.f * m.v() )
				+ src.v() * ( 1.f - dst.a() )
				+ dst.v();
		Pixel c3 = dst.a() * ( 2.f * src.v() - src.a() ) * ( m.v().sqrt() - m.v() )
				+ src.v() * ( 1.f - dst.a() )
				+ dst.v();
		
		Pixel c4 = PixelVec::choose( PixelVec::lessThanEqual( dst.v(), 0.25f * dst.a() ), c2.v(), c3.v() );
		return PixelVec::choose( PixelVec::lessThanEqual( src.v(), 0.5f * src.a() ), c1.v(), c4.v() );
	}
};

struct BlendTraitsDifference : public BlendTraitsSourceOver
{
	static Pixel blend(const Pixel &dst, const Pixel &src)
	{
		Pixel d = src.v() + dst.v() - 2.f * PixelVec::minimum(src.v() * dst.aV(), dst.v() * src.aV());
		d.ra() += src.a() * dst.a();
		return d;
	}
};

struct BlendTraitsExclusion : public BlendTraitsSourceOver
{
	static Pixel blend(const Pixel &dst, const Pixel &src)
	{
		Pixel d = src.v() + dst.v() - 2.f * src.v() * dst.v();
		d.ra() += src.a() * dst.a();
		return d;
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
	
	_defaultBlendOp = _blendOps[BlendMode::SourceOver];
}

BlendOpDictionary _BlendOpDictionary;

BlendOpDictionary *blendOpDictionary()
{
	return &_BlendOpDictionary;
}

}

