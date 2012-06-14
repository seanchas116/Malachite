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

template <class Filler>
void mlFillBitmap(MLBitmap<MLArgb> &bitmap, MLBlendOp *blendOp, agg::rasterizer_scanline_aa<> &rasterizer, Filler &filler)
{
	agg::scanline_p8 sl;
	
	typedef MLImageBaseRenderer<Filler> BaseRenderer;
	typedef MLRenderer<BaseRenderer> Renderer;
	
	BaseRenderer baseRen(bitmap, blendOp, &filler);
	Renderer ren(&baseRen);
	
	agg::render_scanlines(rasterizer, sl, ren);
}

void MLImagePaintEngine::drawPath(const QPainterPath &path)
{
	agg::rasterizer_scanline_aa<> ras;
	
	QPainterPath_vs vs(path);
	ras.add_path(vs);
	
	QTransform transform = _state.brush.transform();
	
	switch (_state.brush.type())
	{
	case MLGlobal::BrushTypeColor:
	{
		
		MLImageColorFiller filler(_state.brush.argb(), _opacity);
		mlFillBitmap(_bitmap, _blendOp, ras, filler);
		break;
	}
	case MLGlobal::BrushTypeImage:
	{
		typedef MLImageImageFiller<MLGlobal::SpreadTypeRepeat> Filler;
		
		Filler filler(_state.brush.image(), QPoint(transform.dx(), transform.dy()));
		mlFillBitmap(_bitmap, _blendOp, ras, filler);
		
		break;
	}
	case MLGlobal::BrushTypeLinearGradient:
	{
		MLLinearColorGradient gradient = _state.brush.linearGradient();
		
		if (mlTransformIsSimilar(transform))
		{
			typedef MLImageGradientFiller<MLColorGradient> Filler;
			MLLineGradientMethod method(gradient.start() * transform, gradient.end() * transform);
			Filler filler(&gradient, &method, _state.brush.spreadType());
			mlFillBitmap(_bitmap, _blendOp, ras, filler);
		}
		else
		{
			typedef MLImageTransformedGradientFiller<MLColorGradient> Filler;
			
			MLLineGradientMethod method(gradient.start(), gradient.end());
			Filler filler(&gradient, &method, _state.brush.spreadType(), transform);
			mlFillBitmap(_bitmap, _blendOp, ras, filler);
		}
		
		break;
	}
	case MLGlobal::BrushTypeRadialGradient:
	{
		MLRadialColorGradient gradient = _state.brush.radialGradient();
		
		if (gradient.center() == gradient.focal())
		{
			// similar transform
			if (mlTransformIsSimilar(transform))
			{
				typedef MLImageGradientFiller<MLColorGradient> Filler;
				
				MLRadialGradientMethod method(gradient.center() * transform, gradient.radius() * transform.m11());
				Filler filler(&gradient, &method, _state.brush.spreadType());
				mlFillBitmap(_bitmap, _blendOp, ras, filler);
			}
			else
			{
				typedef MLImageTransformedGradientFiller<MLColorGradient> Filler;
				
				MLRadialGradientMethod method(gradient.center(), gradient.radius());
				Filler filler(&gradient, &method, _state.brush.spreadType(), transform);
				mlFillBitmap(_bitmap, _blendOp, ras, filler);
			}
		}
		else
		{
			// similar transform
			if (mlTransformIsSimilar(transform))
			{
				typedef MLImageGradientFiller<MLColorGradient> Filler;
				
				MLFocalGradientMethod method(gradient.center() * transform, gradient.radius() * transform.m11(), gradient.focal() * transform);
				Filler filler(&gradient, &method, _state.brush.spreadType());
				mlFillBitmap(_bitmap, _blendOp, ras, filler);
			}
			else
			{
				typedef MLImageTransformedGradientFiller<MLColorGradient> Filler;
				
				MLFocalGradientMethod method(gradient.center(), gradient.radius(), gradient.focal());
				Filler filler(&gradient, &method, _state.brush.spreadType(), transform);
				mlFillBitmap(_bitmap, _blendOp, ras, filler);
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

