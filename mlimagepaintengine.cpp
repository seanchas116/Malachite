#include <QtGui>
#include "agg_rasterizer_scanline_aa.h"
#include "agg_scanline_p.h"
#include "mlimagerenderer.h"
#include "mlimagepaintengine.h"


// return if transform is similar
// currently this function cannot detect similar transforms which contain rotation
bool mlTransformIsSimilar(const QTransform &transform)
{
	return transform.isIdentity() || (transform.isAffine() && transform.m12() == 0 && transform.m21() == 0 && transform.m11() == transform.m22());
}

bool MLImagePaintEngine::begin(MLPaintable *paintable)
{
	_image = dynamic_cast<MLImage *>(paintable);
	Q_ASSERT(_image);
	
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
	_opacity = state.opacity;
}

/*
template <class Rasterizer, class Filler> inline void MLImagePaintEngine::fill(Rasterizer &ras, Filler &filler)
{
	agg::scanline_p8 sl;
	MLImageBaseRenderer<Filler> baseRen(_bitmap, _blendOp, filler);
	MLRenderer<MLImageBaseRenderer<Filler> > ren(baseRen);
	
	agg::render_scanlines(ras, sl, ren);
}

template <MLGlobal::SpreadType SpreadType>
void MLImagePaintEngine::drawPathReal(const QPainterPath &path)
{
	MLBrush brush = _state.brush;
	
	QTransform shapeTransform = _state.worldTransform.inverted();
	QTransform fillShapeTransform = shapeTransform * brush.transform();
	
	QPainterPath newPath = shapeTransform.map(path);
	
	agg::rasterizer_scanline_aa<> ras;
	
	QPainterPath_vs vs(newPath);
	ras.add_path(vs);
	
	if (brush.type() == MLGlobal::BrushTypeColor)
	{
		MLColorFiller filler(brush.argb(), _opacity);
		fill(ras, filler);
	}
	else if (brush.type() == MLGlobal::BrushTypeImage && fillShapeTransform.isIdentity())
	{
		QPoint offset(fillShapeTransform.dx(), fillShapeTransform.dy());
		
		if (fillShapeTransform.isAffine() && fillShapeTransform.m11() == 1 && fillShapeTransform.m12() == 0 && fillShapeTransform.m21() == 0 && fillShapeTransform.m22() == 1 && QPointF(fillShapeTransform.dx(), fillShapeTransform.dy()) == QPointF(offset))
		{
			switch (SpreadType)
			{
			case MLGlobal::SpreadTypePad:
			{
				MLPadImageFiller filler(brush.image().constBitmap(), offset);
				fill(ras, filler);
				break;
			}
			case MLGlobal::SpreadTypeRepeat:
			{
				MLRepeatImageFiller filler(brush.image().constBitmap(), offset);
				fill(ras, filler);
				break;
			}
			case MLGlobal::SpreadTypeReflective:
			{
				MLReflectiveImageFiller filler(brush.image().constBitmap(), offset);
				fill(ras, filler);
				break;
			}
			default:
				return;
			}
		}
	}
	else
	{
		switch (brush.type())
		{
		case MLGlobal::BrushTypeLinearGradient:
		{
			MLLinearGradientInfo info = brush.linearGradientInfo();
			
			if (mlTransformIsSimilar(fillShapeTransform))
			{
				MLLinearGradientMethod method(info.start() * fillShapeTransform, info.end() * fillShapeTransform);
				
				typedef MLGradientFillGenerator<MLColorGradientCache, MLLinearGradientMethod, SpreadType> Generator;
				typedef MLGeneratedFiller<Generator> Filler;
				
				Generator generator(*brush.gradientCache(), method);
				Filler filler(generator, _opacity);
				fill(ras, filler);
			}
			else
			{
				MLLinearGradientMethod method(info.start(), info.end());
				
				typedef MLGradientFillGenerator<MLColorGradientCache, MLLinearGradientMethod, SpreadType> Generator;
				typedef MLGeneratedFiller<Generator> Filler;
				
				Generator generator(*brush.gradientCache(), method);
				Filler filler(generator, _opacity, fillShapeTransform.inverted());
				fill(ras, filler);
			}
			
			break;
		}
		case MLGlobal::BrushTypeRadialGradient:
		{
			MLRadialGradientInfo info = brush.radialGradientInfo();
			
			if (info.center() == info.focal())
			{
				if (mlTransformIsSimilar(fillShapeTransform))
				{
					MLRadialGradientMethod method(fillShapeTransform * info.center(), info.radius() * fillShapeTransform.m11());
					
					typedef MLGradientFillGenerator<MLColorGradientCache, MLRadialGradientMethod, SpreadType> Generator;
					typedef MLGeneratedFiller<Generator> Filler;
					
					Generator generator(*brush.gradientCache(), method);
					Filler filler(generator, _opacity);
					fill(ras, filler);
				}
				else
				{
					MLRadialGradientMethod method(info.center(), info.radius());
					
					typedef MLGradientFillGenerator<MLColorGradientCache, MLRadialGradientMethod, SpreadType> Generator;
					typedef MLGeneratedFiller<Generator> Filler;
					
					Generator generator(*brush.gradientCache(), method);
					Filler filler(generator, _opacity, fillShapeTransform.inverted());
					fill(ras, filler);
				}
			}
			else
			{
				if (mlTransformIsSimilar(fillShapeTransform))
				{
					MLFocalGradientMethod method(fillShapeTransform * info.center(), info.radius() * fillShapeTransform.m11(), info.focal() * fillShapeTransform);
					
					typedef MLGradientFillGenerator<MLColorGradientCache, MLFocalGradientMethod, SpreadType> Generator;
					typedef MLGeneratedFiller<Generator> Filler;
					
					Generator generator(*brush.gradientCache(), method);
					Filler filler(generator, _opacity);
					fill(ras, filler);
				}
				else
				{
					MLFocalGradientMethod method(info.center(), info.radius(), info.focal());
					
					typedef MLGradientFillGenerator<MLColorGradientCache, MLFocalGradientMethod, SpreadType> Generator;
					typedef MLGeneratedFiller<Generator> Filler;
					
					Generator generator(*brush.gradientCache(), method);
					Filler filler(generator, _opacity, fillShapeTransform.inverted());
					fill(ras, filler);
				}
			}
			break;
		}
		default:
			return;
		}
	}
}
*/

void MLImagePaintEngine::drawPath(const QPainterPath &path)
{
	/*
	switch (_state.brush.spreadType())
	{
	case MLGlobal::SpreadTypePad:
		drawPathReal<MLGlobal::SpreadTypePad>(path);
		break;
	case MLGlobal::SpreadTypeReflective:
		drawPathReal<MLGlobal::SpreadTypeReflective>(path);
		break;
	case MLGlobal::SpreadTypeRepeat:
		drawPathReal<MLGlobal::SpreadTypeRepeat>(path);
		break;
	default:
		break;
	}
	*/
	
	const MLBrush brush = _state.brush;
	
	const QTransform shapeTransform = _state.worldTransform.inverted();
	const QTransform fillShapeTransform = brush.transform() * shapeTransform;
	
	QPainterPath newPath = shapeTransform.map(path);
	
	agg::rasterizer_scanline_aa<> ras;
	
	QPainterPath_vs vs(newPath);
	ras.add_path(vs);
	
	MLFiller *filler = 0;
	MLFillGenerator *generator = 0;
	
	if (brush.type() == MLGlobal::BrushTypeColor)
	{
		filler = new MLColorFiller(brush.argb(), _opacity);
	}
	else if (brush.type() == MLGlobal::BrushTypeImage)
	{
		QPoint offset(fillShapeTransform.dx(), fillShapeTransform.dy());
		
		if (fillShapeTransform.isAffine() && fillShapeTransform.m11() == 1 && fillShapeTransform.m12() == 0 && fillShapeTransform.m21() == 0 && fillShapeTransform.m22() == 1 && QPointF(fillShapeTransform.dx(), fillShapeTransform.dy()) == QPointF(offset))
		{
			switch (brush.spreadType())
			{
			case MLGlobal::SpreadTypePad:
				filler = new MLPadImageFiller(brush.image().constBitmap(), offset);
				break;
			case MLGlobal::SpreadTypeRepeat:
				filler = new MLRepeatImageFiller(brush.image().constBitmap(), offset);
				break;
			case MLGlobal::SpreadTypeReflective:
				filler = new MLReflectiveImageFiller(brush.image().constBitmap(), offset);
				break;
			default:
				return;
			}
		}
		else
		{
			switch (_state.imageTransformType)
			{
			case MLGlobal::ImageTransformTypeNearestNeighbor:
				generator = new MLScalingFilterNearestNeighbor<MLBitmap<MLArgb>, true>(brush.image().constBitmap());
				break;
			case MLGlobal::ImageTransformTypeBilinear:
				generator = new MLScalingFilterBilinear<MLBitmap<MLArgb>, true>(brush.image().constBitmap());
				break;
			case MLGlobal::ImageTransformTypeBicubic:
				generator = new MLScalingFilterBicubic<MLBitmap<MLArgb>, true>(brush.image().constBitmap());
				break;
			case MLGlobal::ImageTransformTypeLanczos2:
				generator = new MLScalingFilterLanczos2<MLBitmap<MLArgb>, true>(brush.image().constBitmap());
				break;
			case MLGlobal::ImageTransformTypeLanczos2Hypot:
				generator = new MLScalingFilterLanczos2Hypot<MLBitmap<MLArgb>, true>(brush.image().constBitmap());
				break;
			default:
				return;
			}
			
			filler = new MLGeneratedFiller(*generator, _opacity, fillShapeTransform.inverted());
			generator->setSpreadType(brush.spreadType());
		}
	}
	else
	{
		switch (brush.type())
		{
		case MLGlobal::BrushTypeLinearGradient:
		{
			MLLinearGradientInfo info = brush.linearGradientInfo();
			
			if (mlTransformIsSimilar(fillShapeTransform))
			{
				generator = new MLLinearGradientFillGenerator<MLColorGradientCache>(brush.gradientCache(), info.start() * fillShapeTransform, info.end() * fillShapeTransform);
				filler = new MLGeneratedFiller(*generator, _opacity);
			}
			else
			{
				generator = new MLLinearGradientFillGenerator<MLColorGradientCache>(brush.gradientCache(), info.start(), info.end());
				filler = new MLGeneratedFiller(*generator, _opacity, fillShapeTransform.inverted());
			}
			
			break;
		}
		case MLGlobal::BrushTypeRadialGradient:
		{
			MLRadialGradientInfo info = brush.radialGradientInfo();
			
			if (info.center() == info.focal())
			{
				if (mlTransformIsSimilar(fillShapeTransform))
				{
					generator = new MLRadialGradientFillGenerator<MLColorGradientCache>(brush.gradientCache(), fillShapeTransform * info.center(), info.radius() * fillShapeTransform.m11());
					filler = new MLGeneratedFiller(*generator, _opacity);
				}
				else
				{
					generator = new MLRadialGradientFillGenerator<MLColorGradientCache>(brush.gradientCache(), info.center(), info.radius());
					filler = new MLGeneratedFiller(*generator, _opacity, fillShapeTransform.inverted());
				}
			}
			else
			{
				if (mlTransformIsSimilar(fillShapeTransform))
				{
					generator = new MLFocalGradientFillGenerator<MLColorGradientCache>(brush.gradientCache(), info.center() * fillShapeTransform, info.radius() * fillShapeTransform.m11(), info.focal() * fillShapeTransform);
					filler = new MLGeneratedFiller(*generator, _opacity);
				}
				else
				{
					generator = new MLFocalGradientFillGenerator<MLColorGradientCache>(brush.gradientCache(), info.center(), info.radius(), info.focal());
					filler = new MLGeneratedFiller(*generator, _opacity, fillShapeTransform.inverted());
				}
			}
			break;
		}
		default:
			return;
		}
		generator->setSpreadType(brush.spreadType());
	}
	
	agg::scanline_p8 sl;
	MLImageBaseRenderer baseRen(_bitmap, _blendOp, *filler);
	MLRenderer<MLImageBaseRenderer> ren(baseRen);
	
	agg::render_scanlines(ras, sl, ren);
	
	if (generator)
		delete generator;
	
	if (filler)
		delete filler;
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
		
		if (_opacity == 1.0f)
			_blendOp->blend(targetRect.width(), _bitmap.pixelPointer(p), image.constPixelPointer(p - point));
		else
			_blendOp->blend(targetRect.width(), _bitmap.pixelPointer(p), image.constPixelPointer(p - point), _opacity);
	}
}

