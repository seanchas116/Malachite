#include "mlblendop.h"

inline void blendFuncClear(MLFastArgbF *dst, const MLFastArgbF *src)
{
	Q_UNUSED(src);
	dst->v = MLSimdF4(0);
}

inline MLBlendOp::TileStates tileOpClear(MLBlendOp::TileStates states)
{
	Q_ASSERT(states);
	return MLBlendOp::NoTiles;
}

inline void blendFuncSource(MLFastArgbF *dst, const MLFastArgbF *src)
{
	dst->v = src->v;
}

inline MLBlendOp::TileStates tileOpSource(MLBlendOp::TileStates states)
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

inline void blendFuncDestination(MLFastArgbF *dst, const MLFastArgbF *src)
{
	Q_UNUSED(dst); Q_UNUSED(src);
}

inline MLBlendOp::TileStates tileOpDestination(MLBlendOp::TileStates states)
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

inline void blendFuncSourceOver(MLFastArgbF *dst, const MLFastArgbF *src)
{
	dst->v = src->v + (1.0f - src->a()) * dst->v;
}

inline MLBlendOp::TileStates tileOpSourceOver(MLBlendOp::TileStates states)
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

inline void blendFuncDestinationOver(MLFastArgbF *dst, const MLFastArgbF *src)
{
	dst->v = dst->v + (1.0f - dst->a()) * src->v;
}

inline MLBlendOp::TileStates tileOpDestinationOver(MLBlendOp::TileStates states)
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

inline void blendFuncSourceIn(MLFastArgbF *dst, const MLFastArgbF *src)
{
	dst->v = dst->a() * src->v;
}

inline MLBlendOp::TileStates tileOpSourceIn(MLBlendOp::TileStates states)
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

inline void blendFuncDestinationIn(MLFastArgbF *dst, const MLFastArgbF *src)
{
	dst->v = src->a() * dst->v;
}

inline MLBlendOp::TileStates tileOpDestinationIn(MLBlendOp::TileStates states)
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

inline void blendFuncSourceOut(MLFastArgbF *dst, const MLFastArgbF *src)
{
	dst->v = (1.0f - dst->a()) * src->v;
}


inline MLBlendOp::TileStates tileOpSourceOut(MLBlendOp::TileStates states)
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

inline void blendFuncDestinationOut(MLFastArgbF *dst, const MLFastArgbF *src)
{
	dst->v = (1.0f - src->a()) * dst->v;
}

inline MLBlendOp::TileStates tileOpDestinationOut(MLBlendOp::TileStates states)
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

inline void blendFuncSourceAtop(MLFastArgbF *dst, const MLFastArgbF *src)
{
	dst->v = dst->a() * src->v + (1.0f - src->a()) * dst->v;
}

inline MLBlendOp::TileStates tileOpSourceAtop(MLBlendOp::TileStates states)
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

inline void blendFuncDestinationAtop(MLFastArgbF *dst, const MLFastArgbF *src)
{
	dst->v = src->a() * dst->v + (1.0f - dst->a()) * src->v;
}

inline MLBlendOp::TileStates tileOpDestinationAtop(MLBlendOp::TileStates states)
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

inline void blendFuncXor(MLFastArgbF *dst, const MLFastArgbF *src)
{
	dst->v = (1.0f - dst->a()) * src->v + (1.0f - src->a()) * dst->v;
}

inline MLBlendOp::TileStates tileOpXor(MLBlendOp::TileStates states)
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



MLBlendOpDictionary::MLBlendOpDictionary()
{
	_blendOps[MLGlobal::BlendModeClear] = new MLTemplateBlendOp<blendFuncClear, tileOpClear>;
	_blendOps[MLGlobal::BlendModeSource] = new MLTemplateBlendOp<blendFuncSource, tileOpSource>;
	_blendOps[MLGlobal::BlendModeDestination] = new MLTemplateBlendOp<blendFuncDestination, tileOpDestination>;
	_blendOps[MLGlobal::BlendModeSourceOver] = new MLTemplateBlendOp<blendFuncSourceOver, tileOpSourceOver>;
	_blendOps[MLGlobal::BlendModeDestinationOver] = new MLTemplateBlendOp<blendFuncDestinationOver, tileOpDestinationOver>;
	_blendOps[MLGlobal::BlendModeSourceIn] = new MLTemplateBlendOp<blendFuncSourceIn, tileOpSourceIn>;
	_blendOps[MLGlobal::BlendModeDestinationIn] = new MLTemplateBlendOp<blendFuncDestinationIn, tileOpDestinationIn>;
	_blendOps[MLGlobal::BlendModeSourceOut] = new MLTemplateBlendOp<blendFuncSourceOut, tileOpSourceOut>;
	_blendOps[MLGlobal::BlendModeDestinationOut] = new MLTemplateBlendOp<blendFuncDestinationOut, tileOpDestinationOut>;
	_blendOps[MLGlobal::BlendModeSourceAtop] = new MLTemplateBlendOp<blendFuncSourceAtop, tileOpSourceAtop>;
	_blendOps[MLGlobal::BlendModeDestinationAtop] = new MLTemplateBlendOp<blendFuncDestinationAtop, tileOpDestinationAtop>;
	_blendOps[MLGlobal::BlendModeXor] = new MLTemplateBlendOp<blendFuncXor, tileOpXor>;
	
	_blendOps[MLGlobal::BlendModeNormal] = _blendOps.value(MLGlobal::BlendModeSourceOver);
}

MLBlendOpDictionary BlendOpDictionary;

MLBlendOpDictionary *mlBlendOpDictionary()
{
	return &BlendOpDictionary;
}

