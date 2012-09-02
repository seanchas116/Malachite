#include "mlimagepaintengine.h"
#include "mlfiller.h"
#include "mlgradientgenerator.h"
#include "mlscalinggenerator.h"
#include "mlpainter.h"

template <class Rasterizer, class Filler>
void mlFill(Rasterizer *ras, MLArgbBitmap *bitmap, MLBlendOp *blendOp, Filler *filler)
{
	agg::scanline_pf sl;
	MLImageBaseRenderer<Filler> baseRen(*bitmap, blendOp, filler);
	MLRenderer<MLImageBaseRenderer<Filler> > ren(baseRen);
	
	mlRenderScanlines(*ras, sl, ren);
}

template <class Rasterizer, ML::SpreadType SpreadType, class Source, ML::PixelFieldType SourceType>
void mlDrawTransformedImageBrush(Rasterizer *ras, MLArgbBitmap *bitmap, MLBlendOp *blendOp, const Source &source, float opacity, const QTransform &worldTransform, ML::ImageTransformType transformType)
{
	switch (transformType)
	{
	case ML::ImageTransformTypeNearestNeighbor:
	{
		typedef MLScalingGeneratorNearestNeighbor<Source, SourceType, SpreadType> Generator;
		Generator gen(&source);
		MLFiller<Generator, true> filler(&gen, opacity, worldTransform);
		mlFill(ras, bitmap, blendOp, &filler);
		return;
	}
	case ML::ImageTransformTypeBilinear:
	{
		typedef MLScalingGeneratorBilinear<Source, SourceType, SpreadType> Generator;
		Generator gen(&source);
		MLFiller<Generator, true> filler(&gen, opacity, worldTransform);
		mlFill(ras, bitmap, blendOp, &filler);
		return;
	}
	case ML::ImageTransformTypeBicubic:
	{
		typedef MLScalingGenerator2<Source, SourceType, SpreadType, MLScalingWeightMethodBicubic> Generator;
		Generator gen(&source);
		MLFiller<Generator, true> filler(&gen, opacity, worldTransform);
		mlFill(ras, bitmap, blendOp, &filler);
		return;
	}
	case ML::ImageTransformTypeLanczos2:
	{
		typedef MLScalingGenerator2<Source, SourceType, SpreadType, MLScalingWeightMethodLanczos2> Generator;
		Generator gen(&source);
		MLFiller<Generator, true> filler(&gen, opacity, worldTransform);
		mlFill(ras, bitmap, blendOp, &filler);
		return;
	}
	case ML::ImageTransformTypeLanczos2Hypot:
	{
		typedef MLScalingGenerator2<Source, SourceType, SpreadType, MLScalingWeightMethodLanczos2Hypot> Generator;
		Generator gen(&source);
		MLFiller<Generator, true> filler(&gen, opacity, worldTransform);
		mlFill(ras, bitmap, blendOp, &filler);
		return;
	}
	default:
		return;
	}
}

template <class Rasterizer, ML::SpreadType SpreadType>
void mlDrawWithSpreadType(Rasterizer *ras, MLArgbBitmap *bitmap, MLBlendOp *blendOp, const MLPaintEngineState &state)
{
	const MLBrush brush = state.brush;
	const float opacity = state.opacity;
	
	if (brush.type() == ML::BrushTypeColor)
	{
		MLColorFiller filler(brush.argb(), opacity);
		mlFill(ras, bitmap, blendOp, &filler);
		return;
	}
	
	QTransform fillShapeTransform = brush.transform() * state.shapeTransform;
	
	if (brush.type() == ML::BrushTypeImage)
	{
		if (mlTransformIsIntegerTranslating(fillShapeTransform))
		{
			QPoint offset(fillShapeTransform.dx(), fillShapeTransform.dy());
			
			MLImageFiller<SpreadType> filler(brush.image().constBitmap(), offset);
			mlFill(ras, bitmap, blendOp, &filler);
			return;
		}
		else
		{
			mlDrawTransformedImageBrush<Rasterizer, SpreadType, MLArgbBitmap, ML::PixelFieldImage>(ras, bitmap, blendOp, brush.image().constBitmap(), opacity, fillShapeTransform.inverted(), state.imageTransformType);
			return;
		}
	}
	if (brush.type() == ML::BrushTypeSurface)
	{
		mlDrawTransformedImageBrush<Rasterizer, SpreadType, MLSurface, ML::PixelFieldSurface>(ras, bitmap, blendOp, brush.surface(), opacity, fillShapeTransform.inverted(), state.imageTransformType);
		return;
	}
	if (brush.type() == ML::BrushTypeLinearGradient)
	{
		MLLinearGradientInfo info = brush.linearGradientInfo();
		
		if (info.transformable(fillShapeTransform))
		{
			info.transform(fillShapeTransform);
			fillShapeTransform = QTransform();
		}
		
		if (fillShapeTransform.isIdentity())
		{
			MLLinearGradientMethod method(info.start, info.end);
			MLGradientGenerator<MLColorGradient, MLLinearGradientMethod, SpreadType> gen(brush.gradient(), &method);
			MLFiller<MLGradientGenerator<MLColorGradient, MLLinearGradientMethod, SpreadType>, false> filler(&gen, opacity);
			mlFill(ras, bitmap, blendOp, &filler);
			return;
		}
		else
		{
			MLLinearGradientMethod method(info.start, info.end);
			MLGradientGenerator<MLColorGradient, MLLinearGradientMethod, SpreadType> gen(brush.gradient(), &method);
			MLFiller<MLGradientGenerator<MLColorGradient, MLLinearGradientMethod, SpreadType>, true> filler(&gen, opacity, fillShapeTransform.inverted());
			mlFill(ras, bitmap, blendOp, &filler);
			return;
		}
	}
	if (brush.type() == ML::BrushTypeRadialGradient)
	{
		MLRadialGradientInfo info = brush.radialGradientInfo();
		
		if (info.transformable(fillShapeTransform))
		{
			info.transform(fillShapeTransform);
			fillShapeTransform = QTransform();
		}
		
		if (info.center == info.focal)
		{
			if (fillShapeTransform.isIdentity())
			{
				MLRadialGradientMethod method(info.center, info.radius);
				MLGradientGenerator<MLColorGradient, MLRadialGradientMethod, SpreadType> gen(brush.gradient(), &method);
				MLFiller<MLGradientGenerator<MLColorGradient, MLRadialGradientMethod, SpreadType>, false> filler(&gen, opacity);
				mlFill(ras, bitmap, blendOp, &filler);
				return;
			}
			else
			{
				MLRadialGradientMethod method(info.center, info.radius);
				MLGradientGenerator<MLColorGradient, MLRadialGradientMethod, SpreadType> gen(brush.gradient(), &method);
				MLFiller<MLGradientGenerator<MLColorGradient, MLRadialGradientMethod, SpreadType>, true> filler(&gen, opacity, fillShapeTransform.inverted());
				mlFill(ras, bitmap, blendOp, &filler);
				return;
			}
		}
		else
		{
			if (fillShapeTransform.isIdentity())
			{
				MLFocalGradientMethod method(info.center, info.radius, info.focal);
				MLGradientGenerator<MLColorGradient, MLFocalGradientMethod, SpreadType> gen(brush.gradient(), &method);
				MLFiller<MLGradientGenerator<MLColorGradient, MLFocalGradientMethod, SpreadType>, false> filler(&gen, opacity);
				mlFill(ras, bitmap, blendOp, &filler);
				return;
			}
			else
			{
				MLFocalGradientMethod method(info.center, info.radius, info.focal);
				MLGradientGenerator<MLColorGradient, MLFocalGradientMethod, SpreadType> gen(brush.gradient(), &method);
				MLFiller<MLGradientGenerator<MLColorGradient, MLFocalGradientMethod, SpreadType>, true> filler(&gen, opacity, fillShapeTransform.inverted());
				mlFill(ras, bitmap, blendOp, &filler);
				return;
			}
		}
	}
}




MLImagePaintEngine::MLImagePaintEngine() :
	MLPaintEngine(),
	_image(0)
{}

bool MLImagePaintEngine::begin(MLPaintable *paintable)
{
	_image = dynamic_cast<MLImage *>(paintable);
	if (!_image)
		return false;
	
	_bitmap = _image->bitmap();
	
	return true;
}

bool MLImagePaintEngine::flush()
{
	return true;
}

void MLImagePaintEngine::drawTransformedPolygons(const MLFixedMultiPolygon &polygons)
{
	agg::rasterizer_scanline_aa<> ras;
	
	foreach (const MLFixedPolygon &polygon, polygons)
	{
		if (polygon.size() < 3) continue;
		
		MLFixedPoint p = polygon.at(0);
		ras.move_to(p.x, p.y);
		
		for (int i = 1; i < polygon.size(); ++i)
		{
			p = polygon.at(i);
			ras.line_to(p.x, p.y);
		}
	}
	
	switch (state()->brush.spreadType())
	{
	case ML::SpreadTypePad:
			mlDrawWithSpreadType<agg::rasterizer_scanline_aa<>, ML::SpreadTypePad>(&ras, &_bitmap, state()->blendMode.op(), *state());
		return;
	case ML::SpreadTypeRepeat:
		mlDrawWithSpreadType<agg::rasterizer_scanline_aa<>, ML::SpreadTypeRepeat>(&ras, &_bitmap, state()->blendMode.op(), *state());
		return;
	case ML::SpreadTypeReflective:
		mlDrawWithSpreadType<agg::rasterizer_scanline_aa<>, ML::SpreadTypeReflective>(&ras, &_bitmap, state()->blendMode.op(), *state());
		return;
	default:
		return;
	}
}

void MLImagePaintEngine::drawTransformedImage(const QPoint &point, const MLImage &image)
{
	QRect dstRect = QRect(QPoint(), _image->size());
	QRect srcRect = QRect(point, image.size());
	
	QRect targetRect = dstRect & srcRect;
	
	if (targetRect.isEmpty())
		return;
	
	for (int y = targetRect.top(); y <= targetRect.bottom(); ++y)
	{
		QPoint p(targetRect.left(), y);
		
		if (state()->opacity == 1.0)
			state()->blendMode.op()->blend(targetRect.width(), _bitmap.pixelPointer(p), image.constPixelPointer(p - point));
		else
			state()->blendMode.op()->blend(targetRect.width(), _bitmap.pixelPointer(p), image.constPixelPointer(p - point), state()->opacity);
	}
}


