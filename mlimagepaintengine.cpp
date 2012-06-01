#include <QtGui>
#include "agg_curves.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_scanline_p.h"

#include "mlimagepaintengine.h"

class QPainterPath_agg_vertex_source
{
public:
	QPainterPath_agg_vertex_source(const QPainterPath &path) :
		_path(path),
		_index(0),
		_curve4(0)
	{}
	
	void rewind(unsigned index) { _index = 0; }
	unsigned vertex(double *x, double *y);
	
private:
	const QPainterPath _path;
	int _index;
	agg::curve4 *_curve4;
};

unsigned QPainterPath_agg_vertex_source::vertex(double *x, double *y)
{
	forever {
		
		unsigned type;
		if (_curve4) {	// curveの途中
			type = _curve4->vertex(x, y);
			if (type == agg::path_cmd_move_to)
				continue;	// 最初の頂点なので無視
			if (type != agg::path_cmd_stop)
				return type;	// line_to
			
			// ポリゴン終了
			delete _curve4;
			_curve4 = 0;
		}
		
		if (_index == _path.elementCount())	// path終わり
			return agg::path_cmd_stop;
		
		const QPainterPath::Element element = _path.elementAt(_index);
		
		if (element.type == QPainterPath::CurveToElement) {
			_curve4 = new agg::curve4(_path.elementAt(_index - 1).x, _path.elementAt(_index - 1).y,
									  _path.elementAt(_index).x, _path.elementAt(_index).y,
									  _path.elementAt(_index + 1).x, _path.elementAt(_index + 1).y,
									  _path.elementAt(_index + 2).x, _path.elementAt(_index + 2).y);
			_index += 3;
			continue;
		}
		
		_index++;
		*x = element.x;
		*y = element.y;
		
		switch (element.type) {
		case QPainterPath::MoveToElement:
			return agg::path_cmd_move_to;
		case QPainterPath::LineToElement:
			return agg::path_cmd_line_to;
		default:
			return agg::path_cmd_line_to;
		}
	}
}


template <class BaseRenderer>
class MLRenderer
{
public:
	MLRenderer(BaseRenderer *ren) :
		_ren(ren)
	{}
	
	// from AGG code
    template<class Scanline> 
    void render(const Scanline& sl)
    {
        int y = sl.y();
        unsigned num_spans = sl.num_spans();
        typename Scanline::const_iterator span = sl.begin();

        for(;;)
        {
            int x = span->x;
            if(span->len > 0)
            {
                _ren->blendRasterizerSpan(x, y,span->len, span->covers);
            }
            else
            {
                _ren->blendRasterizerLine(x, y, -span->len, *(span->covers));
            }
            if(--num_spans == 0) break;
            ++span;
        }
    }
	
private:
	BaseRenderer *_ren;
};

// from AGG code
template<class Rasterizer, class Scanline, class Renderer>
void mlRenderScanline(Rasterizer& ras, Scanline& sl, Renderer& ren)
{
    if(ras.rewind_scanlines())
    {
        sl.reset(ras.min_x(), ras.max_x());
        while(ras.sweep_scanline(sl))
        {
            ren.render(sl);
        }
    }
}

template <class Filler>
class MLImageBaseRenderer
{
public:
	MLImageBaseRenderer(const MLBitmap<MLFastArgbF> &bitmap, MLBlendOp *blendOp, Filler *filler) :
		_bitmap(bitmap),
		_blendOp(blendOp),
		_filler(filler)
	{}
	
	void blendRasterizerSpan(int x, int y, int count, const uint8_t *covers)
	{
		if (y < _bitmap.rect().top() || _bitmap.rect().bottom() < y)
			return;
		
		int start = qMax(x, _bitmap.rect().left());
		int end = qMin(x + count, _bitmap.rect().left() + _bitmap.rect().width());
		int newCount = end - start;
		
		float *newCovers = new float[newCount];
		
		for (int i = 0; i < newCount; ++i)
		{
			newCovers[i] = (float)covers[i + start - x] / 255.0f;
		}
		
		_filler->blend(_bitmap, _blendOp, start, y, newCount, newCovers);
		
		delete[] newCovers;
	}
	
	void blendRasterizerLine(int x, int y, int count, uint8_t cover)
	{
		if (y < _bitmap.rect().top() || _bitmap.rect().bottom() < y)
			return;
		
		int start = qMax(x, _bitmap.rect().left());
		int end = qMin(x + count, _bitmap.rect().left() + _bitmap.rect().width());
		unsigned newCount = end - start;
		
		_filler->blend(_bitmap, _blendOp, start, y, newCount, (float)cover / 255.0f);
	}
	
private:
	MLBitmap<MLFastArgbF> _bitmap;
	MLBlendOp *_blendOp;
	Filler *_filler;
};

class MLImageColorFiller
{
public:
	MLImageColorFiller(const MLFastArgbF &argb, double opacity) :
		_argb(argb)
	{
		_argb.v *= mlFloatToVector(opacity);
	}
	
	void blend(MLBitmap<MLFastArgbF> &bitmap, MLBlendOp *blendOp, int x, int y, int count, float *covers)
	{
		blendOp->blend(count, bitmap.pixelPointer(x, y), _argb, covers);
	}
	
	void blend(MLBitmap<MLFastArgbF> &bitmap, MLBlendOp *blendOp, int x, int y, int count, float cover)
	{
		blendOp->blend(count, bitmap.pixelPointer(x, y), _argb, cover);
	}
	
private:
	MLFastArgbF _argb;
};


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

void MLImagePaintEngine::drawPath(const QPainterPath &path)
{
	agg::scanline_p8 sl;
	agg::rasterizer_scanline_aa<> ras;
	
	QPainterPath_agg_vertex_source vs(path);
	ras.add_path(vs);
	
	MLImageColorFiller filler(_state.brush.argb(), _opacity);
	MLImageBaseRenderer<MLImageColorFiller> baseRen(_bitmap, _blendOp, &filler);
	MLRenderer<MLImageBaseRenderer<MLImageColorFiller> > ren(&baseRen);
	
	mlRenderScanline(ras, sl, ren);
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

