#include <QtGui>
#include "agg_rasterizer_scanline_aa.h"
#include "agg_scanline_p.h"
#include "mlimagerenderer.h"
#include "mlimagepaintengine.h"


// return if transform is similar
// currently this function cannot detect similar transforms which contain rotation
bool mlTransformIsSimilar(const QTransform &transform)
{
	return transform.isAffine() && transform.m12() == 0 && transform.m21() == 0 && transform.m11() == transform.m22();
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

template <class Rasterizer, class Filler>
void MLImagePaintEngine::fill(Rasterizer &rasterizer, Filler &filler)
{
	agg::scanline_p8 sl;
	
	typedef MLImageBaseRenderer<Filler> BaseRenderer;
	typedef MLRenderer<BaseRenderer> Renderer;
	
	BaseRenderer baseRen(_bitmap, _blendOp, &filler);
	Renderer ren(&baseRen);
	
	agg::render_scanlines(rasterizer, sl, ren);
}

template <class Rasterizer> void MLImagePaintEngine::fillImage(Rasterizer &ras, const MLImage &image, MLGlobal::SpreadType spreadType, const QTransform &transform)
{
	QPoint offset(transform.dx(), transform.dy());
	
	if (transform.isTranslating() && QPointF(transform.dx(), transform.dy()) == QPointF(offset))
	{
		switch (spreadType)
		{
		case MLGlobal::SpreadTypePad:
		{
			MLPadImageFiller filler(image, offset);
			fill(ras, filler);
			break;
		}
		case MLGlobal::SpreadTypeRepeat:
		{
			MLRepeatImageFiller filler(image, offset);
			fill(ras, filler);
			break;
		}
		case MLGlobal::SpreadTypeReflective:
		{
			MLReflectiveImageFiller filler(image, offset);
			fill(ras, filler);
			break;
		}
		default:
			break;
		}
	}
	else
	{
		switch (spreadType)
		{
		case MLGlobal::SpreadTypePad:
		{
			MLTransformedImageFiller<MLGlobal::SpreadTypePad> filler(image, transform);
			fill(ras, filler);
			break;
		}
		case MLGlobal::SpreadTypeRepeat:
		{
			MLTransformedImageFiller<MLGlobal::SpreadTypeRepeat> filler(image, transform);
			fill(ras, filler);
			break;
		}
		case MLGlobal::SpreadTypeReflective:
		{
			MLTransformedImageFiller<MLGlobal::SpreadTypeReflective> filler(image, transform);
			fill(ras, filler);
			break;
		}
		default:
			break;
		}
	}
}


template <class Rasterizer, class Gradient, class Method>
void MLImagePaintEngine::fillGradient(Rasterizer &ras, Gradient *gradient, const Method &method, MLGlobal::SpreadType spreadType)
{
	switch (spreadType)
	{
	case MLGlobal::SpreadTypePad:
	{
		MLGradientFiller<Gradient, Method, MLGlobal::SpreadTypePad> filler(gradient, &method);
		fill(ras, filler);
		break;
	}
	case MLGlobal::SpreadTypeRepeat:
	{
		MLGradientFiller<Gradient, Method, MLGlobal::SpreadTypeRepeat> filler(gradient, &method);
		fill(ras, filler);
		break;
	}
	case MLGlobal::SpreadTypeReflective:
	{
		MLGradientFiller<Gradient, Method, MLGlobal::SpreadTypeReflective> filler(gradient, &method);
		fill(ras, filler);
		break;
	}
	default:
		break;
	}
}

template <class Rasterizer, class Gradient, class Method>
void MLImagePaintEngine::fillGradient(Rasterizer &ras, Gradient *gradient, const Method &method, MLGlobal::SpreadType spreadType, const QTransform &transform)
{
	switch (spreadType)
	{
	case MLGlobal::SpreadTypePad:
	{
		MLTransformedGradientFiller<Gradient, Method, MLGlobal::SpreadTypePad> filler(gradient, &method, transform);
		fill(ras, filler);
		break;
	}
	case MLGlobal::SpreadTypeRepeat:
	{
		MLTransformedGradientFiller<Gradient, Method, MLGlobal::SpreadTypeRepeat> filler(gradient, &method, transform);
		fill(ras, filler);
		break;
	}
	case MLGlobal::SpreadTypeReflective:
	{
		MLTransformedGradientFiller<Gradient, Method, MLGlobal::SpreadTypeReflective> filler(gradient, &method, transform);
		fill(ras, filler);
		break;
	}
	default:
		break;
	}
}

void MLImagePaintEngine::drawPath(const QPainterPath &path)
{
	agg::rasterizer_scanline_aa<> ras;
	
	QPainterPath_vs vs(path);
	ras.add_path(vs);
	
	MLBrush brush = _state.brush;
	QTransform transform = brush.transform();
	
	switch (brush.type())
	{
	case MLGlobal::BrushTypeColor:
	{
		MLColorFiller filler(brush.argb(), _opacity);
		fill(ras, filler);
		break;
	}
	case MLGlobal::BrushTypeImage:
	{
		fillImage(ras, brush.image(), brush.spreadType(), transform);
		break;
	}
	case MLGlobal::BrushTypeLinearGradient:
	{
		MLLinearGradientInfo info = brush.linearGradientInfo();
		
		if (mlTransformIsSimilar(transform))
		{
			MLLinearGradientMethod method(info.start() * transform, info.end() * transform);
			fillGradient(ras, brush.gradientCache(), method, brush.spreadType());
		}
		else
		{
			MLLinearGradientMethod method(info.start(), info.end());
			fillGradient(ras, brush.gradientCache(), method, brush.spreadType(), transform);
		}
		
		break;
	}
	case MLGlobal::BrushTypeRadialGradient:
	{
		MLRadialGradientInfo info = brush.radialGradientInfo();
		
		if (info.center() == info.focal())
		{
			// similar transform
			if (mlTransformIsSimilar(transform))
			{
				MLRadialGradientMethod method(info.center() * transform, info.radius() * transform.m11());
				fillGradient(ras, brush.gradientCache(), method, brush.spreadType());
			}
			else
			{
				MLRadialGradientMethod method(info.center(), info.radius());
				fillGradient(ras, brush.gradientCache(), method, brush.spreadType(), transform);
			}
		}
		else
		{
			// similar transform
			if (mlTransformIsSimilar(transform))
			{
				MLFocalGradientMethod method(info.center() * transform, info.radius() * transform.m11(), info.focal() * transform);
				fillGradient(ras, brush.gradientCache(), method, brush.spreadType());
			}
			else
			{
				MLFocalGradientMethod method(info.center(), info.radius(), info.focal());
				fillGradient(ras, brush.gradientCache(), method, brush.spreadType(), transform);
			}
		}
		break;
	}
	default:
		break;
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
		
		if (_opacity == 1.0f)
			_blendOp->blend(targetRect.width(), _bitmap.pixelPointer(p), image.constPixelPointer(p - point));
		else
			_blendOp->blend(targetRect.width(), _bitmap.pixelPointer(p), image.constPixelPointer(p - point), _opacity);
	}
}

