#include "blendmode.h"
#include "blendop.h"

// Pixel blend mode based on SVG compositing specification

namespace Malachite
{

static const PixelVec pixelVecZero(0.f);
static const PixelVec pixelVecOne(1.f);

inline static void loadPixels
(
	PixelVec &da, PixelVec &dr, PixelVec &dg, PixelVec &db, 
	const PixelVec &s0, const PixelVec &s1, const PixelVec &s2, const PixelVec &s3
)
{
	PixelVec v0 = _mm_unpackhi_ps(s0, s1); // (a0 r0 g0 b0), (a1 r1 g1 b1) => (a1 a0 r1 r0)
	PixelVec v1 = _mm_unpackhi_ps(s2, s3); // (a2 r2 g2 b2), (a3 r3 g3 b3) => (a3 a2 r3 r2)
	PixelVec v2 = _mm_unpacklo_ps(s0, s1); // (a0 r0 g0 b0), (a1 r1 g1 b1) => (g1 g0 b1 b0)
	PixelVec v3 = _mm_unpacklo_ps(s2, s3); // (a2 r2 g2 b2), (a3 r3 g3 b3) => (g3 g2 b3 b2)
	
	da = _mm_movehl_ps(v1, v0); // (a3 a2 a1 a0)
	dr = _mm_movelh_ps(v0, v1); // (r3 r2 r1 r0)
	dg = _mm_movehl_ps(v3, v2); // (g3 g2 g1 g0)
	db = _mm_movelh_ps(v2, v3); // (b3 b2 b1 b0)
}

inline static void loadAlphas
(
	PixelVec &da,
	const PixelVec &s0, const PixelVec &s1, const PixelVec &s2, const PixelVec &s3
)
{
	PixelVec v0 = _mm_unpackhi_ps(s0, s1);	// a1 a0 r1 r0
	PixelVec v1 = _mm_unpackhi_ps(s2, s3);	// a3 a2 r3 r2
	da = _mm_movehl_ps(v1, v0);
}

inline static void savePixels
(
	PixelVec &d0, PixelVec &d1, PixelVec &d2, PixelVec &d3,
	const PixelVec &sa, const PixelVec &sr, const PixelVec &sg, const PixelVec &sb
)
{
	PixelVec v0 = _mm_unpacklo_ps(sr, sa);	// a1 r1 a0 r0
	PixelVec v1 = _mm_unpackhi_ps(sr, sa); // a3 r3 a2 r2
	PixelVec v2 = _mm_unpacklo_ps(sb, sg); // g1 b1 g0 b0
	PixelVec v3 = _mm_unpackhi_ps(sb, sg); // g3 b3 g2 b2
	
	d0 = _mm_movelh_ps(v2, v0);
	d1 = _mm_movehl_ps(v0, v2);
	d2 = _mm_movelh_ps(v3, v1);
	d3 = _mm_movehl_ps(v1, v3);
}

struct EmptyCoverIter
{
	EmptyCoverIter &operator+=(int) { return *this; }
};

template <typename T>
struct FixedBlendIter
{
	using value_type = T;
	
	FixedBlendIter(const T &value) : value(value) {}
	
	FixedBlendIter &operator+=(int) { return *this; }
	T operator[](int) const { return value; }
	
	T value;
};

template <typename T>
inline static FixedBlendIter<T> makeFixedBlendIter(const T &value)
{
	return FixedBlendIter<T>(value);
}

template <typename TIter>
struct ReverseBlendIter
{
	using value_type = typename TIter::value_type;
	
	ReverseBlendIter(const TIter &i) : iter(i) {}
	
	ReverseBlendIter &operator+=(int diff)
	{
		iter -= diff;
		return *this;
	}
	
	value_type operator[](int index) const { return iter[-index]; }
	
	TIter iter;
};

template <typename TIter>
inline static ReverseBlendIter<TIter> makeReverseBlendIter(const TIter &iter)
{
	return ReverseBlendIter<TIter>(iter);
}

template <typename TCoverIter>
struct BlendCoverTraits;

template <>
struct BlendCoverTraits<Pointer<const Pixel> >
{
	using Iterator = Pointer<const Pixel>;
	
	static void initializeOpacities(PixelVec &, const Iterator &) {}
	
	static void updateOpacities(PixelVec &opacities, const Iterator &iter)
	{
		loadAlphas(opacities, iter[0].v(), iter[1].v(), iter[2].v(), iter[3].v());
	}
	
	static void updateOpacitiesRemaining(PixelVec &opacities, const Iterator &iter, int count)
	{
		int i1 = std::min(1, count-1);
		int i2 = std::min(2, count-1);
		int i3 = std::min(3, count-1);
		loadAlphas(opacities, iter[0].v(), iter[i1].v(), iter[i2].v(), iter[i3].v());
	}
	
	static void applyOpacities(PixelVec &colors, const PixelVec &opacities)
	{
		colors *= opacities;
	}
};

template <>
struct BlendCoverTraits<Pointer<const float> >
{
	using Iterator = Pointer<const float>;
	
	static void initializeOpacities(PixelVec &, const Iterator &) {}
	
	static void updateOpacities(PixelVec &opacities, const Iterator &iter)
	{
		opacities = _mm_setr_ps(iter[0], iter[1], iter[2], iter[3]);
	}
	
	static void updateOpacitiesRemaining(PixelVec &opacities, const Iterator &iter, int count)
	{
		int i1 = std::min(1, count-1);
		int i2 = std::min(2, count-1);
		int i3 = std::min(3, count-1);
		opacities = _mm_setr_ps(iter[0], iter[i1], iter[i2], iter[i3]);
	}
	
	static void applyOpacities(PixelVec &colors, const PixelVec &opacities)
	{
		colors *= opacities;
	}
};

template <>
struct BlendCoverTraits<FixedBlendIter<Pixel> >
{
	using Iterator = FixedBlendIter<Pixel>;
	
	static void initializeOpacities(PixelVec &opacities, const Iterator &iter)
	{
		loadAlphas(opacities, iter[0].v(), iter[0].v(), iter[0].v(), iter[0].v());
	}
	
	static void updateOpacities(PixelVec &, const Iterator &) {}
	static void updateOpacitiesRemaining(PixelVec &, const Iterator &, int) {}
	
	static void applyOpacities(PixelVec &colors, const PixelVec &opacities)
	{
		colors *= opacities;
	}
};

template <>
struct BlendCoverTraits<FixedBlendIter<float> >
{
	using Iterator = FixedBlendIter<float>;
	
	static void initializeOpacities(PixelVec &opacities, const Iterator &iter)
	{
		opacities = _mm_setr_ps(iter[0], iter[0], iter[0], iter[0]);
	}
	
	static void updateOpacities(PixelVec &, const Iterator &) {}
	static void updateOpacitiesRemaining(PixelVec &, const Iterator &, int) {}
	
	static void applyOpacities(PixelVec &colors, const PixelVec &opacities)
	{
		colors *= opacities;
	}
};

template <>
struct BlendCoverTraits<EmptyCoverIter>
{
	using Iterator = EmptyCoverIter;
	
	static void initializeOpacities(PixelVec &, const Iterator &) {}
	static void updateOpacities(PixelVec &, const Iterator &) {}
	static void updateOpacitiesRemaining(PixelVec &, const Iterator &, int) {}
	static void applyOpacities(PixelVec &, const PixelVec &) {}
};

template <typename TBlendTraits, bool TIsSeparable = TBlendTraits::isSeparable()>
struct ColorBlendCall;

template <typename TBlendTraits>
struct ColorBlendCall<TBlendTraits, true>
{
	static PixelVec blendAlpha(const PixelVec &da, const PixelVec &sa)
	{
		return TBlendTraits::blendAlpha(da, sa);
	}
	
	static void blendColor
	(
		PixelVec &rr, PixelVec &rg, PixelVec &rb,
		const PixelVec &da, const PixelVec &dr, const PixelVec &dg, const PixelVec &db,
		const PixelVec &sa, const PixelVec &sr, const PixelVec &sg, const PixelVec &sb
	)
	{
		rr = TBlendTraits::blendComponent(da, dr, sa, sr);
		rg = TBlendTraits::blendComponent(da, dg, sa, sg);
		rb = TBlendTraits::blendComponent(da, db, sa, sb);
	}
};

template <typename TBlendTraits>
struct ColorBlendCall<TBlendTraits, false>
{
	static PixelVec blendAlpha(const PixelVec &da, const PixelVec &sa)
	{
		return TBlendTraits::blendAlpha(da, sa);
	}
	
	static void blendColor
	(
		PixelVec &rr, PixelVec &rg, PixelVec &rb,
		const PixelVec &da, const PixelVec &dr, const PixelVec &dg, const PixelVec &db,
		const PixelVec &sa, const PixelVec &sr, const PixelVec &sg, const PixelVec &sb
	)
	{
		TBlendTraits::blendColor(rr, rg, rb, da, dr, dg, db, sa, sr, sg, sb);
	}
};

template <typename TBlendTraits, typename TDstIter, typename TSrcIter, typename TCoverIter>
void blendMain(int count, TDstIter dst, TSrcIter src, TCoverIter covers)
{
	using CoverTraits = BlendCoverTraits<TCoverIter>;
	
	PixelVec o;
	CoverTraits::initializeOpacities(o, covers);
	
	while (count >= 4)
	{
		PixelVec da, dr, dg, db;
		loadPixels(da, dr, dg, db, dst[0], dst[1], dst[2], dst[3]);
		
		PixelVec sa, sr, sg, sb;
		loadPixels(sa, sr, sg, sb, src[0], src[1], src[2], src[3]);
		
		CoverTraits::updateOpacities(o, covers);
		CoverTraits::applyOpacities(sa, o);
		CoverTraits::applyOpacities(sr, o);
		CoverTraits::applyOpacities(sg, o);
		CoverTraits::applyOpacities(sb, o);
		
		PixelVec ra = TBlendTraits::blendAlpha(da, sa);
		PixelVec rr, rg, rb;
		ColorBlendCall<TBlendTraits>::blendColor(rr, rg, rb, da, dr, dg, db, sa, sr, sg, sb);
		
		savePixels(dst[0], dst[1], dst[2], dst[3], ra, rr, rg, rb);
		
		count -= 4;
		dst += 4;
		src += 4;
		covers += 4;
	}
	if (count)
	{
		int i1 = std::min(1, count-1);
		int i2 = std::min(2, count-1);
		int i3 = std::min(3, count-1);
		
		PixelVec da, dr, dg, db;
		loadPixels(da, dr, dg, db, dst[0], dst[i1], dst[i2], dst[i3]);
		
		PixelVec sa, sr, sg, sb;
		loadPixels(sa, sr, sg, sb, src[0], src[i1], src[i2], src[i3]);
		
		CoverTraits::updateOpacitiesRemaining(o, covers, count);
		CoverTraits::applyOpacities(sa, o);
		CoverTraits::applyOpacities(sr, o);
		CoverTraits::applyOpacities(sg, o);
		CoverTraits::applyOpacities(sb, o);
		
		PixelVec ra = TBlendTraits::blendAlpha(da, sa);
		PixelVec rr, rg, rb;
		ColorBlendCall<TBlendTraits>::blendColor(rr, rg, rb, da, dr, dg, db, sa, sr, sg, sb);
		
		savePixels(dst[0], dst[i1], dst[i2], dst[i3], ra, rr, rg, rb);
	}
}

template <typename TBlendTraits>
class TemplateBlendOp : public BlendOp
{
public:
	void blend(int count, Pointer<Pixel> dst, Pointer<const Pixel> src)
	{
		blendMain<TBlendTraits>(count, dst, src, EmptyCoverIter());
	}
	
	void blend(int count, Pointer<Pixel> dst, Pointer<const Pixel> src, Pointer<const Pixel> masks)
	{
		blendMain<TBlendTraits>(count, dst, src, masks);
	}

	void blend(int count, Pointer<Pixel> dst, Pointer<const Pixel> src, Pointer<const float> opacities)
	{
		blendMain<TBlendTraits>(count, dst, src, opacities);
	}

	void blend(int count, Pointer<Pixel> dst, Pointer<const Pixel> src, const Pixel &mask)
	{
		blendMain<TBlendTraits>(count, dst, src, makeFixedBlendIter(mask));
	}

	void blend(int count, Pointer<Pixel> dst, Pointer<const Pixel> src, float opacity)
	{
		blendMain<TBlendTraits>(count, dst, src, makeFixedBlendIter(opacity));
	}
	
	void blend(int count, Pointer<Pixel> dst, const Pixel &src)
	{
		blendMain<TBlendTraits>(count, dst, makeFixedBlendIter(src), EmptyCoverIter());
	}
	
	void blend(int count, Pointer<Pixel> dst, const Pixel &src, Pointer<const Pixel> masks)
	{
		blendMain<TBlendTraits>(count, dst, makeFixedBlendIter(src), masks);
	}
	
	void blend(int count, Pointer<Pixel> dst, const Pixel &src, Pointer<const float> opacities)
	{
		blendMain<TBlendTraits>(count, dst, makeFixedBlendIter(src), opacities);
	}
	
	void blendReversed(int count, Pointer<Pixel> dst, Pointer<const Pixel> src)
	{
		src += count - 1;
		blendMain<TBlendTraits>(count, dst, makeReverseBlendIter(src), EmptyCoverIter());
	}
	
	void blendReversed(int count, Pointer<Pixel> dst, Pointer<const Pixel> src, Pointer<const Pixel> masks)
	{
		src += count - 1;
		blendMain<TBlendTraits>(count, dst, makeReverseBlendIter(src), masks);
	}
	
	void blendReversed(int count, Pointer<Pixel> dst, Pointer<const Pixel> src, Pointer<const float> opacities)
	{
		src += count - 1;
		blendMain<TBlendTraits>(count, dst, makeReverseBlendIter(src), opacities);
	}

	void blendReversed(int count, Pointer<Pixel> dst, Pointer<const Pixel> src, const Pixel &mask)
	{
		src += count - 1;
		blendMain<TBlendTraits>(count, dst, makeReverseBlendIter(src), makeFixedBlendIter(mask));
	}
	
	void blendReversed(int count, Pointer<Pixel> dst, Pointer<const Pixel> src, float opacity)
	{
		src += count - 1;
		blendMain<TBlendTraits>(count, dst, makeReverseBlendIter(src), makeFixedBlendIter(opacity));
	}
	
	TileCombination tileRequirement(TileCombination combination)
	{
		return TBlendTraits::tileRequirement(combination);
	}
};

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
	static void blend(Pixel &dst, const Pixel &src) { Q_UNUSED(dst); Q_UNUSED(src); }
	
	static BlendOp::TileCombination tileRequirement(BlendOp::TileCombination states)
	{
		switch (states)
		{
		case BlendOp::TileBoth:
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
	
	static constexpr bool isSeparable()
	{
		return true;
	}
	
	static PixelVec blendAlpha(const PixelVec &da, const PixelVec &sa)
	{
		return da + sa - da * sa;
	}
	
	static PixelVec blendComponent(const PixelVec &da, const PixelVec &dc, const PixelVec &sa, const PixelVec &sc)
	{
		Q_UNUSED(da);
		return sc + (pixelVecOne - sa) * dc;
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
		c3.rv() = dst.a() * ( 2.f * src.v() - src.a() ) * ( m.v().sqrt() - m.v() )
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
	_blendOps[BlendMode::SourceOver] = new TemplateBlendOp<BlendTraitsSourceOver>;
	
	/*
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
	*/
	_defaultBlendOp = _blendOps[BlendMode::SourceOver];
}

BlendOpDictionary _BlendOpDictionary;

BlendOpDictionary *blendOpDictionary()
{
	return &_BlendOpDictionary;
}

}

