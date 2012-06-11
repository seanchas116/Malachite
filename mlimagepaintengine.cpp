#include <QtGui>
#include "agg_rasterizer_scanline_aa.h"
#include "agg_scanline_p.h"
#include "mlimagerenderer.h"
#include "mlimagepaintengine.h"


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
		
		MLBrush brush = _state.brush;
		
		Filler filler(brush.image(), QPoint(brush.transform().dx(), brush.transform().dy()));
		mlFillBitmap(_bitmap, _blendOp, ras, filler);
		
		break;
	}
	case MLGlobal::BrushTypeGradient:
	{
		MLColorGradient aGradient = _state.brush.gradient();
		QTransform transform = _state.brush.transform();
		
		switch (_state.brush.gradient().type())
		{
		case MLGlobal::GradientTypeLinear:
		{
			MLLinearColorGradient gradient = *(static_cast<MLLinearColorGradient *>(&aGradient));
			
			if (transform.isAffine())
			{
				typedef MLImageGradientFiller<MLColorGradient, MLLineGradientMethod> Filler;
				
				MLLineGradientMethod method(gradient.start() * transform, gradient.end() * transform);
				Filler filler(gradient, method);
				mlFillBitmap(_bitmap, _blendOp, ras, filler);
			}
			else
			{
				typedef MLImageTransformedGradientFiller<MLColorGradient, MLLineGradientMethod> Filler;
				
				MLLineGradientMethod method(gradient.start(), gradient.end());
				Filler filler(gradient, method, transform);
				mlFillBitmap(_bitmap, _blendOp, ras, filler);
			}
			
			break;
		}
		case MLGlobal::GradientTypeRadial:
		{
			MLRadialColorGradient gradient = *(static_cast<MLRadialColorGradient *>(&aGradient));
			
			if (gradient.center() == gradient.focal())
			{
				// similar transform
				if (transform.isAffine() && transform.m12() == 0 && transform.m21() == 0 && transform.m11() == transform.m22())
				{
					typedef MLImageGradientFiller<MLColorGradient, MLRadialGradientMethod> Filler;
					
					MLRadialGradientMethod method(gradient.center() * transform, gradient.radius() * transform.m11());
					Filler filler(gradient, method);
					mlFillBitmap(_bitmap, _blendOp, ras, filler);
				}
				else
				{
					typedef MLImageTransformedGradientFiller<MLColorGradient, MLRadialGradientMethod> Filler;
					
					MLRadialGradientMethod method(gradient.center(), gradient.radius());
					Filler filler(gradient, method, transform);
					mlFillBitmap(_bitmap, _blendOp, ras, filler);
				}
			}
			else
			{
				// similar transform
				if (transform.isAffine() && transform.m12() == 0 && transform.m21() == 0 && transform.m11() == transform.m22())
				{
					typedef MLImageGradientFiller<MLColorGradient, MLFocalGradientMethod> Filler;
					
					MLFocalGradientMethod method(gradient.center() * transform, gradient.radius() * transform.m11(), gradient.focal() * transform);
					Filler filler(gradient, method);
					mlFillBitmap(_bitmap, _blendOp, ras, filler);
				}
				else
				{
					typedef MLImageTransformedGradientFiller<MLColorGradient, MLFocalGradientMethod> Filler;
					
					MLFocalGradientMethod method(gradient.center(), gradient.radius(), gradient.focal());
					Filler filler(gradient, method, transform);
					mlFillBitmap(_bitmap, _blendOp, ras, filler);
				}
			}
			break;
		}
		default:
			break;
		}
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

