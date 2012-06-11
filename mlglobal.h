#ifndef MLGLOBAL_H
#define MLGLOBAL_H

#include <Qt>
#include <QEvent>
#include <QMetaType>

#if defined(MALACHITE_LIBRARY)
#  define MALACHITESHARED_EXPORT Q_DECL_EXPORT
#else
#  define MALACHITESHARED_EXPORT Q_DECL_IMPORT
#endif

namespace MLGlobal
{

enum BlendMode
{
	BlendModeNormal = 0,	// 通常
	BlendModePlus,	// 加算
	BlendModeMultiply,	// 乗算
	BlendModeScreen,	// スクリーン
	BlendModeOverlay,	// オーバーレイ
	BlendModeDarken,	// 比較（暗）
	BlendModeLighten,	// 比較（明）
	BlendModeColorDodge,	// 覆い焼き
	BlendModeColorBurn,	// 焼き込み
	BlendModeHardLight,	// ハードライト
	BlendModeSoftLight,	// ソフトライト
	BlendModeDifference,	// 差の絶対値
	BlendModeExclusion,	// 除外
	
	BlendModeClear,
	BlendModeSource,
	BlendModeDestination,
	BlendModeSourceOver,
	BlendModeDestinationOver,
	BlendModeSourceIn,
	BlendModeDestinationIn,
	BlendModeSourceOut,
	BlendModeDestinationOut,
	BlendModeSourceAtop,
	BlendModeDestinationAtop,
	BlendModeXor,
	
	BlendModePassThrough	// Group only
};

enum ArgbPosition
{
	ArgbAlpha = 3,
	ArgbRed = 2,
	ArgbGreen = 1,
	ArgbBlue = 0
};

enum RgbaPosition
{
	RgbaRed = 3,
	RgbaGreen = 2,
	RgbaBlue = 1,
	RgbaAlpha = 0
};

enum RgbPositoon
{
	RgbRed = 2,
	RgbGreen = 1,
	RgbBlue = 0
};

enum BrushType
{
	BrushTypeNull,
	BrushTypeColor,
	BrushTypeGradient,
	BrushTypeImage,
	BrushTypeSurface
};

enum SpreadType
{
	SpreadTypePad,
	SpreadTypeRepeat,
	SpreadTypeReflective
};

enum GradientType
{
	GradientTypeNull,
	GradientTypeLinear,
	GradientTypeRadial
};

}

#endif // MLGLOBAL_H
