#include "mlimagepaintenginev2.h"
#include "mlimagerendererv2.h"

template <class Rasterizer, class Filler>
void mlFill(Rasterizer *ras, MLArgbBitmap *bitmap, MLBlendOp *blendOp, Filler *filler)
{
	agg::scanline_p8 sl;
	MLImageBaseRenderer<Filler> baseRen(*bitmap, blendOp, filler);
	MLRenderer<MLImageBaseRenderer<Filler> > ren(baseRen);
	
	mlRenderScanlines(*ras, sl, ren);
}

bool mlTransformIsIntegerTranslating(const QTransform &transform)
{
	return transform.isTranslating() && transform.dx() == int(transform.dx()) && transform.dy() == int(transform.dy());
}

bool mlTransformIsSimilar(const QTransform &transform)
{
	return transform.isIdentity() || (transform.isAffine() && transform.m12() == 0 && transform.m21() == 0 && transform.m11() == transform.m22());
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
	
	const QTransform fillShapeTransform = brush.transform() * state.shapeTransform;
	
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
	else if (brush.type() == ML::BrushTypeLinearGradient)
	{
		MLLinearGradientInfo info = brush.linearGradientInfo();
		
		if (mlTransformIsSimilar(fillShapeTransform))
		{
			MLLinearGradientMethod method(info.start() * fillShapeTransform, info.end() * fillShapeTransform);
			MLGradientGenerator<MLColorGradient, MLLinearGradientMethod, SpreadType> gen(brush.gradient(), &method);
			MLFiller<MLGradientGenerator<MLColorGradient, MLLinearGradientMethod, SpreadType>, false> filler(&gen, opacity);
			mlFill(ras, bitmap, blendOp, &filler);
			return;
		}
		else
		{
			MLLinearGradientMethod method(info.start(), info.end());
			MLGradientGenerator<MLColorGradient, MLLinearGradientMethod, SpreadType> gen(brush.gradient(), &method);
			MLFiller<MLGradientGenerator<MLColorGradient, MLLinearGradientMethod, SpreadType>, true> filler(&gen, opacity, fillShapeTransform.inverted());
			mlFill(ras, bitmap, blendOp, &filler);
			return;
		}
	}
	else if (brush.type() == ML::BrushTypeRadialGradient)
	{
		MLRadialGradientInfo info = brush.radialGradientInfo();
		
		if (info.center() == info.focal())
		{
			if (mlTransformIsSimilar(fillShapeTransform))
			{
				MLRadialGradientMethod method(info.center() * fillShapeTransform, info.radius() * fillShapeTransform.m11());
				MLGradientGenerator<MLColorGradient, MLRadialGradientMethod, SpreadType> gen(brush.gradient(), &method);
				MLFiller<MLGradientGenerator<MLColorGradient, MLRadialGradientMethod, SpreadType>, false> filler(&gen, opacity);
				mlFill(ras, bitmap, blendOp, &filler);
				return;
			}
			else
			{
				MLRadialGradientMethod method(info.center(), info.radius());
				MLGradientGenerator<MLColorGradient, MLRadialGradientMethod, SpreadType> gen(brush.gradient(), &method);
				MLFiller<MLGradientGenerator<MLColorGradient, MLRadialGradientMethod, SpreadType>, true> filler(&gen, opacity, fillShapeTransform.inverted());
				mlFill(ras, bitmap, blendOp, &filler);
				return;
			}
		}
		else
		{
			if (mlTransformIsSimilar(fillShapeTransform))
			{
				MLFocalGradientMethod method(info.center() * fillShapeTransform, info.radius() * fillShapeTransform.m11(), info.focal() * fillShapeTransform);
				MLGradientGenerator<MLColorGradient, MLFocalGradientMethod, SpreadType> gen(brush.gradient(), &method);
				MLFiller<MLGradientGenerator<MLColorGradient, MLFocalGradientMethod, SpreadType>, false> filler(&gen, opacity);
				mlFill(ras, bitmap, blendOp, &filler);
				return;
			}
			else
			{
				MLFocalGradientMethod method(info.center(), info.radius(), info.focal());
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
	_blendOp(0),
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

void MLImagePaintEngine::updateState(const MLPaintEngineState &state)
{
	_state = state;
	_blendOp = state.blendMode.op();
}

void MLImagePaintEngine::drawPath(const QPainterPath &path)
{
	QPainterPath newPath = _state.shapeTransform.map(path);
	
	agg::rasterizer_scanline_aa<> ras;
	QPainterPath_vs vs(newPath);
	ras.add_path(vs);
	
	switch (_state.brush.spreadType())
	{
	case ML::SpreadTypePad:
		mlDrawWithSpreadType<agg::rasterizer_scanline_aa<>, ML::SpreadTypePad>(&ras, &_bitmap, _blendOp, _state);
		return;
	case ML::SpreadTypeRepeat:
		mlDrawWithSpreadType<agg::rasterizer_scanline_aa<>, ML::SpreadTypeRepeat>(&ras, &_bitmap, _blendOp, _state);
		return;
	case ML::SpreadTypeReflective:
		mlDrawWithSpreadType<agg::rasterizer_scanline_aa<>, ML::SpreadTypeReflective>(&ras, &_bitmap, _blendOp, _state);
		return;
	default:
		return;
	}
}

void MLImagePaintEngine::drawImage(const QPoint &point, const MLImage &image)
{
	QRect dstRect = QRect(QPoint(), _image->size());
	QRect srcRect = QRect(point, image.size());
	
	QRect targetRect = dstRect & srcRect;
	
	if (targetRect.isEmpty())
		return;
	
	for (int y = targetRect.top(); y <= targetRect.bottom(); ++y)
	{
		QPoint p(targetRect.left(), y);
		
		if (_state.opacity == 1.0)
			_blendOp->blend(targetRect.width(), _bitmap.pixelPointer(p), image.constPixelPointer(p - point));
		else
			_blendOp->blend(targetRect.width(), _bitmap.pixelPointer(p), image.constPixelPointer(p - point), _state.opacity);
	}
}

