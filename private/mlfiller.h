#ifndef MLFILLER_H
#define MLFILLER_H

#include "mlvector.h"
#include "mlblendop.h"
#include "mlintdivision.h"

class MLColorFiller
{
public:
	MLColorFiller(const MLVec4F &argb, double opacity) :
		_argb(argb * opacity)
	{}
	
	void fill(const QPoint &pos, int count, MLPointer<MLVec4F> dst, MLPointer<float> covers, MLBlendOp *blendOp)
	{
		Q_UNUSED(pos);
		blendOp->blend(count, dst, _argb, covers);
	}
	
	void fill(const QPoint &pos, int count, MLPointer<MLVec4F> dst, float cover, MLBlendOp *blendOp)
	{
		Q_UNUSED(pos);
		blendOp->blend(count, dst, _argb * cover);
	}
	
private:
	MLVec4F _argb;
};

template <ML::SpreadType SpreadType>
class MLImageFiller
{
public:
	
	MLImageFiller(const MLArgbBitmap &bitmap, const QPoint &offset) :
		_srcBitmap(bitmap), _offset(offset) {}
	
	void fill(const QPoint &pos, int count, MLPointer<MLVec4F> dst, MLPointer<float> covers, MLBlendOp *blendOp)
	{
		fillTemplate<false>(pos, count, dst, covers, blendOp);
	}
	
	void fill(const QPoint &pos, int count, MLPointer<MLVec4F> dst, float cover, MLBlendOp *blendOp)
	{
		fillTemplate<true>(pos, count, dst, MLPointer<float>(cover), blendOp);
	}
	
private:
	
	template <bool CoverSingle>
	void fillTemplate(const QPoint &pos, int count, MLPointer<MLVec4F> dst, MLPointer<float> covers, MLBlendOp *blendOp)
	{
		switch (SpreadType)
		{
		case ML::SpreadTypePad:
			fillTemplatePad<CoverSingle>(pos, count, dst, covers, blendOp);
			return;
		case ML::SpreadTypeRepeat:
			fillTemplateRepeat<CoverSingle>(pos, count, dst, covers, blendOp);
			return;
		case ML::SpreadTypeReflective:
			fillTemplateReflective<CoverSingle>(pos, count, dst, covers, blendOp);
			return;
		}
	}
	
	template <bool CoverSingle>
	void fillTemplatePad(const QPoint &pos, int count, MLPointer<MLVec4F> dst, MLPointer<float> covers, MLBlendOp *blendOp)
	{
		QPoint srcPos = pos - _offset;
		int imageY = qBound(0, srcPos.y(), _srcBitmap.height() - 1);
		
		int i = 0;
		
		int left = -srcPos.x();
		
		if (left > 0)
		{
			if (CoverSingle)
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
			if (CoverSingle)
				blendOp->blend(mid, dst + i, _srcBitmap.constPixelPointer(midStart, imageY), *covers);
			else
				blendOp->blend(mid, dst + i, _srcBitmap.constPixelPointer(midStart, imageY), covers + i);
			i += mid;
		}
		
		int right = srcPos.x() + count - _srcBitmap.width();
		
		if (right > 0)
		{
			if (CoverSingle)
				blendOp->blend(mid, dst + i, _srcBitmap.pixel(_srcBitmap.width() - 1, imageY), *covers);
			else
				blendOp->blend(mid, dst + i, _srcBitmap.pixel(_srcBitmap.width() - 1, imageY), covers + i);
		}
	}
	
	template <bool CoverSingle>
	void fillTemplateRepeat(const QPoint &pos, int count, MLPointer<MLVec4F> dst, MLPointer<float> covers, MLBlendOp *blendOp)
	{
		QPoint srcPos = pos - _offset;
		MLIntDivision divX(srcPos.x(), _srcBitmap.width());
		MLIntDivision divY(srcPos.y(), _srcBitmap.height());
		
		int imageY = divY.rem();
		
		int i = _srcBitmap.width() - divX.rem();
		
		if (i >= count)
		{
			if (CoverSingle)
				blendOp->blend(count, dst, _srcBitmap.constPixelPointer(divX.rem(), imageY), *covers);
			else
				blendOp->blend(count, dst, _srcBitmap.constPixelPointer(divX.rem(), imageY), covers);
			return;
		}
		
		if (CoverSingle)
			blendOp->blend(i, dst, _srcBitmap.constPixelPointer(divX.rem(), imageY), *covers);
		else
			blendOp->blend(i, dst, _srcBitmap.constPixelPointer(divX.rem(), imageY), covers);
		
		forever
		{
			if (count - i < _srcBitmap.width()) break;
			
			if (CoverSingle)
				blendOp->blend(_srcBitmap.width(), dst + i, _srcBitmap.constPixelPointer(0, imageY), *covers);
			else
				blendOp->blend(_srcBitmap.width(), dst + i, _srcBitmap.constPixelPointer(0, imageY), covers + i);
			
			i += _srcBitmap.width();
		}
		
		if (CoverSingle)
			blendOp->blend(count - i, dst + i, _srcBitmap.constPixelPointer(0, imageY), *covers);
		else
			blendOp->blend(count - i, dst + i, _srcBitmap.constPixelPointer(0, imageY), covers + i);
	}
	
	template <bool CoverSingle>
	void fillTemplateReflective(const QPoint &pos, int count, MLPointer<MLVec4F> dst, MLPointer<float> covers, MLBlendOp *blendOp)
	{
		QPoint srcPos = pos - _offset;
		MLIntDivision divX(srcPos.x(), _srcBitmap.width());
		MLIntDivision divY(srcPos.y(), _srcBitmap.height());
		
		int imageY = divY.rem();
		
		int i = _srcBitmap.width() - divX.rem();
		
		if (i >= count)
		{
			if (CoverSingle)
				blendOp->blend(count, dst, _srcBitmap.constPixelPointer(divX.rem(), imageY), *covers);
			else
				blendOp->blend(count, dst, _srcBitmap.constPixelPointer(divX.rem(), imageY), covers);
			return;
		}
		
		if (CoverSingle)
			blendOp->blend(i, dst, _srcBitmap.constPixelPointer(divX.rem(), imageY), *covers);
		else
			blendOp->blend(i, dst, _srcBitmap.constPixelPointer(divX.rem(), imageY), covers);
		
		forever
		{
			if (count - i < _srcBitmap.width()) break;
			
			if (CoverSingle)
				blendOp->blend(_srcBitmap.width(), dst + i, _srcBitmap.constPixelPointer(0, imageY), *covers);
			else
				blendOp->blend(_srcBitmap.width(), dst + i, _srcBitmap.constPixelPointer(0, imageY), covers + i);
			
			i += _srcBitmap.width();
		}
		
		if (CoverSingle)
			blendOp->blend(count - i, dst + i, _srcBitmap.constPixelPointer(0, imageY), *covers);
		else
			blendOp->blend(count - i, dst + i, _srcBitmap.constPixelPointer(0, imageY), covers + i);
	}
	
	const MLBitmap<MLVec4F> _srcBitmap;
	QPoint _offset;
};

template <class Generator, bool TransformEnabled>
class MLFiller
{
public:
	MLFiller(Generator *generator, float opacity, const QTransform &worldTransform = QTransform()) :
		_generator(generator),
		_opacity(opacity),
		_transform(worldTransform)
	{}
	
	void fill(const QPoint &pos, int count, MLPointer<MLVec4F> dst, MLPointer<float> covers, MLBlendOp *blendOp)
	{
		if (_opacity != 1)
		{
			for (int i = 0; i < count; ++i)
				covers[i] *= _opacity;
		}
		
		MLArray<MLVec4F> fill(count);
		
		MLVec2D centerPos(pos.x(), pos.y());
		centerPos += MLVec2D(0.5, 0.5);
		
		for (int i = 0; i < count; ++i)
		{
			fill[i] = _generator->at(TransformEnabled ? centerPos * _transform : centerPos);
			centerPos += MLVec2D(1, 0);
		}
		
		blendOp->blend(count, dst, fill.data(), covers);
	}
	
	void fill(const QPoint &pos, int count, MLPointer<MLVec4F> dst, float cover, MLBlendOp *blendOp)
	{
		cover *= _opacity;
		
		MLArray<MLVec4F> fill(count);
		
		MLVec2D centerPos(pos.x(), pos.y());
		centerPos += MLVec2D(0.5, 0.5);
		
		for (int i = 0; i < count; ++i)
		{
			fill[i] = _generator->at(TransformEnabled ? centerPos * _transform : centerPos);
			centerPos += MLVec2D(1, 0);
		}
		
		blendOp->blend(count, dst, fill.data(), cover);
	}
	
private:
	Generator *_generator;
	float _opacity;
	QTransform _transform;
};

#endif // MLFILLER_H
