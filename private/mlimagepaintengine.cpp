#include "mlimagepaintengine.h"
#include "mlfiller.h"
#include "mlgradientgenerator.h"
#include "mlscalinggenerator.h"
#include "mlpainter.h"

namespace Malachite
{

template <class Rasterizer, class Filler>
void fill(Rasterizer *ras, ArgbBitmap *bitmap, BlendOp *blendOp, Filler *filler)
{
	agg::scanline_pf sl;
	ImageBaseRenderer<Filler> baseRen(*bitmap, blendOp, filler);
	Renderer<ImageBaseRenderer<Filler> > ren(baseRen);
	
	renderScanlines(*ras, sl, ren);
}

template <class Rasterizer, Malachite::SpreadType SpreadType, class Source, Malachite::PixelFieldType SourceType>
void drawTransformedImageBrush(Rasterizer *ras, ArgbBitmap *bitmap, BlendOp *blendOp, const Source &source, float opacity, const QTransform &worldTransform, Malachite::ImageTransformType transformType)
{
	switch (transformType)
	{
	case Malachite::ImageTransformTypeNearestNeighbor:
	{
		typedef ScalingGeneratorNearestNeighbor<Source, SourceType, SpreadType> Generator;
		Generator gen(&source);
		Filler<Generator, true> filler(&gen, opacity, worldTransform);
		fill(ras, bitmap, blendOp, &filler);
		return;
	}
	case Malachite::ImageTransformTypeBilinear:
	{
		typedef ScalingGeneratorBilinear<Source, SourceType, SpreadType> Generator;
		Generator gen(&source);
		Filler<Generator, true> filler(&gen, opacity, worldTransform);
		fill(ras, bitmap, blendOp, &filler);
		return;
	}
	case Malachite::ImageTransformTypeBicubic:
	{
		typedef ScalingGenerator2<Source, SourceType, SpreadType, ScalingWeightMethodBicubic> Generator;
		Generator gen(&source);
		Filler<Generator, true> filler(&gen, opacity, worldTransform);
		fill(ras, bitmap, blendOp, &filler);
		return;
	}
	case Malachite::ImageTransformTypeLanczos2:
	{
		typedef ScalingGenerator2<Source, SourceType, SpreadType, ScalingWeightMethodLanczos2> Generator;
		Generator gen(&source);
		Filler<Generator, true> filler(&gen, opacity, worldTransform);
		fill(ras, bitmap, blendOp, &filler);
		return;
	}
	case Malachite::ImageTransformTypeLanczos2Hypot:
	{
		typedef ScalingGenerator2<Source, SourceType, SpreadType, ScalingWeightMethodLanczos2Hypot> Generator;
		Generator gen(&source);
		Filler<Generator, true> filler(&gen, opacity, worldTransform);
		fill(ras, bitmap, blendOp, &filler);
		return;
	}
	default:
		return;
	}
}

template <class T_Rasterizer, Malachite::SpreadType T_SpreadType>
void drawWithSpreadType(T_Rasterizer *ras, ArgbBitmap *bitmap, BlendOp *blendOp, const PaintEngineState &state)
{
	const Brush brush = state.brush;
	const float opacity = state.opacity;
	
	if (brush.type() == Malachite::BrushTypeColor)
	{
		ColorFiller filler(brush.argb(), opacity);
		fill(ras, bitmap, blendOp, &filler);
		return;
	}
	
	QTransform fillShapeTransform = brush.transform() * state.shapeTransform;
	
	if (brush.type() == Malachite::BrushTypeImage)
	{
		if (transformIsIntegerTranslating(fillShapeTransform))
		{
			QPoint offset(fillShapeTransform.dx(), fillShapeTransform.dy());
			
			ImageFiller<T_SpreadType> filler(brush.image().constBitmap(), offset);
			fill(ras, bitmap, blendOp, &filler);
			return;
		}
		else
		{
			drawTransformedImageBrush<T_Rasterizer, T_SpreadType, ArgbBitmap, Malachite::PixelFieldImage>(ras, bitmap, blendOp, brush.image().constBitmap(), opacity, fillShapeTransform.inverted(), state.imageTransformType);
			return;
		}
	}
	if (brush.type() == Malachite::BrushTypeSurface)
	{
		drawTransformedImageBrush<T_Rasterizer, T_SpreadType, Surface, Malachite::PixelFieldSurface>(ras, bitmap, blendOp, brush.surface(), opacity, fillShapeTransform.inverted(), state.imageTransformType);
		return;
	}
	if (brush.type() == Malachite::BrushTypeLinearGradient)
	{
		LinearGradientInfo info = brush.linearGradientInfo();
		
		if (info.transformable(fillShapeTransform))
		{
			info.transform(fillShapeTransform);
			fillShapeTransform = QTransform();
		}
		
		if (fillShapeTransform.isIdentity())
		{
			LinearGradientMethod method(info.start, info.end);
			GradientGenerator<ColorGradient, LinearGradientMethod, T_SpreadType> gen(brush.gradient(), &method);
			Filler<GradientGenerator<ColorGradient, LinearGradientMethod, T_SpreadType>, false> filler(&gen, opacity);
			fill(ras, bitmap, blendOp, &filler);
			return;
		}
		else
		{
			LinearGradientMethod method(info.start, info.end);
			GradientGenerator<ColorGradient, LinearGradientMethod, T_SpreadType> gen(brush.gradient(), &method);
			Filler<GradientGenerator<ColorGradient, LinearGradientMethod, T_SpreadType>, true> filler(&gen, opacity, fillShapeTransform.inverted());
			fill(ras, bitmap, blendOp, &filler);
			return;
		}
	}
	if (brush.type() == Malachite::BrushTypeRadialGradient)
	{
		RadialGradientInfo info = brush.radialGradientInfo();
		
		if (info.transformable(fillShapeTransform))
		{
			info.transform(fillShapeTransform);
			fillShapeTransform = QTransform();
		}
		
		if (info.center == info.focal)
		{
			if (fillShapeTransform.isIdentity())
			{
				RadialGradientMethod method(info.center, info.radius);
				GradientGenerator<ColorGradient, RadialGradientMethod, T_SpreadType> gen(brush.gradient(), &method);
				Filler<GradientGenerator<ColorGradient, RadialGradientMethod, T_SpreadType>, false> filler(&gen, opacity);
				fill(ras, bitmap, blendOp, &filler);
				return;
			}
			else
			{
				RadialGradientMethod method(info.center, info.radius);
				GradientGenerator<ColorGradient, RadialGradientMethod, T_SpreadType> gen(brush.gradient(), &method);
				Filler<GradientGenerator<ColorGradient, RadialGradientMethod, T_SpreadType>, true> filler(&gen, opacity, fillShapeTransform.inverted());
				fill(ras, bitmap, blendOp, &filler);
				return;
			}
		}
		else
		{
			if (fillShapeTransform.isIdentity())
			{
				FocalGradientMethod method(info.center, info.radius, info.focal);
				GradientGenerator<ColorGradient, FocalGradientMethod, T_SpreadType> gen(brush.gradient(), &method);
				Filler<GradientGenerator<ColorGradient, FocalGradientMethod, T_SpreadType>, false> filler(&gen, opacity);
				fill(ras, bitmap, blendOp, &filler);
				return;
			}
			else
			{
				FocalGradientMethod method(info.center, info.radius, info.focal);
				GradientGenerator<ColorGradient, FocalGradientMethod, T_SpreadType> gen(brush.gradient(), &method);
				Filler<GradientGenerator<ColorGradient, FocalGradientMethod, T_SpreadType>, true> filler(&gen, opacity, fillShapeTransform.inverted());
				fill(ras, bitmap, blendOp, &filler);
				return;
			}
		}
	}
}




ImagePaintEngine::ImagePaintEngine() :
	PaintEngine(),
	_image(0)
{}

bool ImagePaintEngine::begin(Paintable *paintable)
{
	_image = dynamic_cast<Image *>(paintable);
	if (!_image)
		return false;
	
	_bitmap = _image->bitmap();
	
	return true;
}

bool ImagePaintEngine::flush()
{
	return true;
}

void ImagePaintEngine::drawTransformedPolygons(const FixedMultiPolygon &polygons)
{
	agg::rasterizer_scanline_aa<> ras;
	
	foreach (const FixedPolygon &polygon, polygons)
	{
		if (polygon.size() < 3) continue;
		
		FixedPoint p = polygon.at(0);
		ras.move_to(p.x, p.y);
		
		for (int i = 1; i < polygon.size(); ++i)
		{
			p = polygon.at(i);
			ras.line_to(p.x, p.y);
		}
	}
	
	BlendOp *op = BlendModeUtil(state()->blendMode).op();
	
	switch (state()->brush.spreadType())
	{
		case Malachite::SpreadTypePad:
			drawWithSpreadType<agg::rasterizer_scanline_aa<>, Malachite::SpreadTypePad>(&ras, &_bitmap, op, *state());
			return;
		case Malachite::SpreadTypeRepeat:
			drawWithSpreadType<agg::rasterizer_scanline_aa<>, Malachite::SpreadTypeRepeat>(&ras, &_bitmap, op, *state());
			return;
		case Malachite::SpreadTypeReflective:
			drawWithSpreadType<agg::rasterizer_scanline_aa<>, Malachite::SpreadTypeReflective>(&ras, &_bitmap, op, *state());
			return;
		default:
			return;
	}
}

void ImagePaintEngine::drawTransformedImage(const QPoint &point, const Image &image)
{
	QRect dstRect = QRect(QPoint(), _image->size());
	QRect srcRect = QRect(point, image.size());
	
	QRect targetRect = dstRect & srcRect;
	
	if (targetRect.isEmpty())
		return;
	
	BlendOp *op = BlendModeUtil(state()->blendMode).op();
	if (!op)
		return;
	
	for (int y = targetRect.top(); y <= targetRect.bottom(); ++y)
	{
		QPoint p(targetRect.left(), y);
		
		if (state()->opacity == 1.0)
			op->blend(targetRect.width(), _bitmap.pixelPointer(p), image.constPixelPointer(p - point));
		else
			op->blend(targetRect.width(), _bitmap.pixelPointer(p), image.constPixelPointer(p - point), state()->opacity);
	}
}

}

