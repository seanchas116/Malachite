#ifndef MLGLOBAL_H
#define MLGLOBAL_H

//ExportName: Global

#include <Qt>
#include <QEvent>
#include <QMetaType>

#if defined(MALACHITE_LIBRARY)
#  define MALACHITESHARED_EXPORT Q_DECL_EXPORT
#else
#  define MALACHITESHARED_EXPORT Q_DECL_IMPORT
#endif

namespace Malachite
{

/**
  This enum describes the blend mode (aka compositing mode) in painting.
  Malachite supports all compositing operations of SVG Compositing Specification (http://www.w3.org/TR/SVGCompositing/) and some original modes.
*/
enum BlendMode
{
	/**
	  Normal (通常)
	  Equal to BlendModeSourceover
	*/
	BlendModeNormal = 0,
	
	/**
	  Plus (加算)
	*/
	BlendModePlus,
	
	/**
	  Multiply (乗算)
	*/
	BlendModeMultiply,
	
	/**
	  Screen (スクリーン)
	*/
	BlendModeScreen,
	
	/**
	  Overlay (オーバーレイ)
	*/
	BlendModeOverlay,
	
	/**
	  Darken (比較（暗）)
	*/
	BlendModeDarken,
	
	/**
	  Lighten (比較（明）)
	*/
	BlendModeLighten,
	
	/**
	  Color Dodge (覆い焼き)
	*/
	BlendModeColorDodge,
	
	/**
	  Color Burn (焼き込み)
	*/
	BlendModeColorBurn,
	
	/**
	  Hard Light (ハードライト)
	*/
	BlendModeHardLight,
	
	/**
	  Soft Light (ソフトライト)
	*/
	BlendModeSoftLight,
	
	/**
	  Difference (差の絶対値)
	*/
	BlendModeDifference,
	
	/**
	  Exclusion (除外)
	*/
	BlendModeExclusion,
	
	BlendModeSourcePadding,
	BlendModeDestinationPadding,
	
	BlendModeClear = 128,
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
	
	/**
	  Pass Through
	  This value is for group layers only.
	  This value means that the group layer will not affect layer blending order.
	*/
	BlendModePassThrough = 256
};

enum ImageFormat
{
	ImageFormatArgb,
	ImageFormatArgbFast,
	ImageFormatRgb
};

enum ImageComponentType
{
	ImageComponentTypeFloat,
	ImageComponentTypeUint16,
	ImageComponentTypeUint8
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
	BrushTypeLinearGradient,
	BrushTypeRadialGradient,
	BrushTypeImage,
	BrushTypeSurface
};

enum SpreadType
{
	SpreadTypeWindowed,
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

enum ImageTransformType
{
	ImageTransformTypeNearestNeighbor,
	ImageTransformTypeBilinear,
	ImageTransformTypeBicubic,
	ImageTransformTypeLanczos2,
	ImageTransformTypeLanczos2Hypot
};

enum PixelFieldType
{
	PixelFieldImage,
	PixelFieldSurface
};

}

#endif // MLGLOBAL_H
