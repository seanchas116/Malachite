#ifndef MLFILLER_H
#define MLFILLER_H

#include "../vector.h"
#include "../blendop.h"
#include "../division.h"

namespace Malachite
{

class ColorFiller
{
public:
	ColorFiller(const Vec4F &argb, double opacity) :
		_argb(argb * opacity)
	{}
	
	void fill(const QPoint &pos, int count, Pointer<Vec4F> dst, Pointer<float> covers, BlendOp *blendOp)
	{
		Q_UNUSED(pos);
		blendOp->blend(count, dst, _argb, covers);
	}
	
	void fill(const QPoint &pos, int count, Pointer<Vec4F> dst, float cover, BlendOp *blendOp)
	{
		Q_UNUSED(pos);
		blendOp->blend(count, dst, _argb * cover);
	}
	
private:
	Vec4F _argb;
};

template <Malachite::SpreadType T_SpreadType>
class ImageFiller
{
public:
	
	ImageFiller(const ArgbBitmap &bitmap, const QPoint &offset) :
		_srcBitmap(bitmap), _offset(offset) {}
	
	void fill(const QPoint &pos, int count, Pointer<Vec4F> dst, Pointer<float> covers, BlendOp *blendOp)
	{
		fillTemplate<false>(pos, count, dst, covers, blendOp);
	}
	
	void fill(const QPoint &pos, int count, Pointer<Vec4F> dst, float cover, BlendOp *blendOp)
	{
		fillTemplate<true>(pos, count, dst, Pointer<float>(cover), blendOp);
	}
	
private:
	
	template <bool CoverIsNotArray>
	void fillTemplate(const QPoint &pos, int count, Pointer<Vec4F> dst, Pointer<float> covers, BlendOp *blendOp)
	{
		switch (T_SpreadType)
		{
		case Malachite::SpreadTypePad:
			fillTemplatePad<CoverIsNotArray>(pos, count, dst, covers, blendOp);
			return;
		case Malachite::SpreadTypeRepeat:
			fillTemplateRepeat<CoverIsNotArray>(pos, count, dst, covers, blendOp);
			return;
		case Malachite::SpreadTypeReflective:
			fillTemplateReflective<CoverIsNotArray>(pos, count, dst, covers, blendOp);
			return;
		}
	}
	
	template <bool CoverIsNotArray>
	void fillTemplatePad(const QPoint &pos, int count, Pointer<Vec4F> dst, Pointer<float> covers, BlendOp *blendOp)
	{
		QPoint srcPos = pos - _offset;
		int imageY = qBound(0, srcPos.y(), _srcBitmap.height() - 1);
		
		int i = 0;
		
		int left = -srcPos.x();
		
		if (left > 0)
		{
			if (CoverIsNotArray)
				blendOp->blend(left, dst, _srcBitmap.pixel(0, imageY), *covers);
			else
				blendOp->blend(left, dst, _srcBitmap.pixel(0, imageY), covers);
			i += left;
		}
		
		int midStart = qMax(0, srcPos.x());
		int midEnd = qMin(_srcBitmap.width(), srcPos.x() + count);
		int mid = midEnd - midStart;
		
		if (mid > 0)
		{
			if (CoverIsNotArray)
				blendOp->blend(mid, dst + i, _srcBitmap.constPixelPointer(midStart, imageY), *covers);
			else
				blendOp->blend(mid, dst + i, _srcBitmap.constPixelPointer(midStart, imageY), covers + i);
			i += mid;
		}
		
		int right = srcPos.x() + count - _srcBitmap.width();
		
		if (right > 0)
		{
			if (CoverIsNotArray)
				blendOp->blend(right, dst + i, _srcBitmap.pixel(_srcBitmap.width() - 1, imageY), *covers);
			else
				blendOp->blend(right, dst + i, _srcBitmap.pixel(_srcBitmap.width() - 1, imageY), covers + i);
		}
	}
	
	template <bool CoverIsNotArray>
	void fillTemplateRepeat(const QPoint &pos, int count, Pointer<Vec4F> dst, Pointer<float> covers, BlendOp *blendOp)
	{
		QPoint srcPos = pos - _offset;
		IntDivision divX(srcPos.x(), _srcBitmap.width());
		IntDivision divY(srcPos.y(), _srcBitmap.height());
		
		int imageY = divY.rem();
		
		int i = _srcBitmap.width() - divX.rem();
		
		if (i >= count)
		{
			if (CoverIsNotArray)
				blendOp->blend(count, dst, _srcBitmap.constPixelPointer(divX.rem(), imageY), *covers);
			else
				blendOp->blend(count, dst, _srcBitmap.constPixelPointer(divX.rem(), imageY), covers);
			return;
		}
		
		if (CoverIsNotArray)
			blendOp->blend(i, dst, _srcBitmap.constPixelPointer(divX.rem(), imageY), *covers);
		else
			blendOp->blend(i, dst, _srcBitmap.constPixelPointer(divX.rem(), imageY), covers);
		
		forever
		{
			if (count - i < _srcBitmap.width()) break;
			
			if (CoverIsNotArray)
				blendOp->blend(_srcBitmap.width(), dst + i, _srcBitmap.constPixelPointer(0, imageY), *covers);
			else
				blendOp->blend(_srcBitmap.width(), dst + i, _srcBitmap.constPixelPointer(0, imageY), covers + i);
			
			i += _srcBitmap.width();
		}
		
		if (CoverIsNotArray)
			blendOp->blend(count - i, dst + i, _srcBitmap.constPixelPointer(0, imageY), *covers);
		else
			blendOp->blend(count - i, dst + i, _srcBitmap.constPixelPointer(0, imageY), covers + i);
	}
	
	template <bool CoverIsNotArray>
	void fillTemplateReflective(const QPoint &pos, int count, Pointer<Vec4F> dst, Pointer<float> covers, BlendOp *blendOp)
	{
		QPoint srcPos = pos - _offset;
		IntDivision divX(srcPos.x(),  _srcBitmap.width());
		IntDivision divY(srcPos.y(),  _srcBitmap.height());
		
		int imageY = divY.quot() % 2 ?  _srcBitmap.height() - divY.rem() - 1 : divY.rem();
		
		int i = _srcBitmap.width() - divX.rem();
		
		int q = divX.quot();
		
		if (i >= count)
		{
			if (q % 2)
			{
				if (CoverIsNotArray)
					blendOp->blendReversed(count, dst, _srcBitmap.constPixelPointer( _srcBitmap.width() - divX.rem() - count, imageY), *covers);
				else
					blendOp->blendReversed(count, dst, _srcBitmap.constPixelPointer( _srcBitmap.width() - divX.rem() - count, imageY), covers);
			}
			else
			{
				if (CoverIsNotArray)
					blendOp->blend(count, dst, _srcBitmap.constPixelPointer(divX.rem(), imageY), *covers);
				else
					blendOp->blend(count, dst, _srcBitmap.constPixelPointer(divX.rem(), imageY), covers);
			}
			return;
		}
		
		if (q % 2)
		{
			if (CoverIsNotArray)
				blendOp->blendReversed(i, dst, _srcBitmap.constPixelPointer(0, imageY), *covers);
			else
				blendOp->blendReversed(i, dst, _srcBitmap.constPixelPointer(0, imageY), covers);
		}
		else
		{
			if (CoverIsNotArray)
				blendOp->blend(i, dst, _srcBitmap.constPixelPointer(divX.rem(), imageY), *covers);
			else
				blendOp->blend(i, dst, _srcBitmap.constPixelPointer(divX.rem(), imageY), covers);
		}
		
		q++;
		
		forever
		{
			if (count - i < _srcBitmap.width()) break;
			
			if (q % 2)
			{
				if (CoverIsNotArray)
					blendOp->blendReversed(_srcBitmap.width(), dst + i, _srcBitmap.constPixelPointer(0, imageY), *covers);
				else
					blendOp->blendReversed(_srcBitmap.width(), dst + i, _srcBitmap.constPixelPointer(0, imageY), covers + i);
			}
			else
			{
				if (CoverIsNotArray)
					blendOp->blend(_srcBitmap.width(), dst + i, _srcBitmap.constPixelPointer(0, imageY), *covers);
				else
					blendOp->blend(_srcBitmap.width(), dst + i, _srcBitmap.constPixelPointer(0, imageY), covers + i);
			}
			i += _srcBitmap.width();
			q++;
		}
		
		if (q % 2)
		{
			if (CoverIsNotArray)
				blendOp->blendReversed(count - i, dst + i, _srcBitmap.constPixelPointer(_srcBitmap.width() - count + i, imageY), *covers);
			else
				blendOp->blendReversed(count - i, dst + i, _srcBitmap.constPixelPointer(_srcBitmap.width() - count + i, imageY), covers + i);
		}
		else
		{
			if (CoverIsNotArray)
				blendOp->blend(count - i, dst + i, _srcBitmap.constPixelPointer(0, imageY), *covers);
			else
				blendOp->blend(count - i, dst + i, _srcBitmap.constPixelPointer(0, imageY), covers + i);
		}
	}
	
	const Bitmap<Vec4F> _srcBitmap;
	QPoint _offset;
};

template <class T_Generator, bool TransformEnabled>
class Filler
{
public:
	Filler(T_Generator *generator, float opacity, const QTransform &worldTransform = QTransform()) :
		_generator(generator),
		_opacity(opacity),
		_transform(worldTransform)
	{}
	
	void fill(const QPoint &pos, int count, Pointer<Vec4F> dst, Pointer<float> covers, BlendOp *blendOp)
	{
		if (_opacity != 1)
		{
			for (int i = 0; i < count; ++i)
				covers[i] *= _opacity;
		}
		
		Array<Vec4F> fill(count);
		
		Vec2D centerPos(pos.x(), pos.y());
		centerPos += Vec2D(0.5, 0.5);
		
		for (int i = 0; i < count; ++i)
		{
			fill[i] = _generator->at(TransformEnabled ? centerPos * _transform : centerPos);
			centerPos += Vec2D(1, 0);
		}
		
		blendOp->blend(count, dst, fill.data(), covers);
	}
	
	void fill(const QPoint &pos, int count, Pointer<Vec4F> dst, float cover, BlendOp *blendOp)
	{
		cover *= _opacity;
		
		Array<Vec4F> fill(count);
		
		Vec2D centerPos(pos.x(), pos.y());
		centerPos += Vec2D(0.5, 0.5);
		
		for (int i = 0; i < count; ++i)
		{
			fill[i] = _generator->at(TransformEnabled ? centerPos * _transform : centerPos);
			centerPos += Vec2D(1, 0);
		}
		
		blendOp->blend(count, dst, fill.data(), cover);
	}
	
private:
	T_Generator *_generator;
	float _opacity;
	QTransform _transform;
};

}

#endif // MLFILLER_H
