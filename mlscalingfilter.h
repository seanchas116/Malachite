#ifndef MLSCALINGFILTER_H
#define MLSCALINGFILTER_H

#include "mlcolor.h"
#include "mlbitmap.h"

template <MLGlobal::SpreadType SpreadType>
class MLScalingFilterLanczos2Hypot
{
public:
	
	MLScalingFilterLanczos2Hypot(MLBitmap<MLArgb> *bitmap) :
		_bitmap(bitmap)
	{
		reset();
	}
	
	MLArgb at(const QPointF &p)
	{
		reset(p);
		
		QPointF fp = mlPointFloor(p);
		
		addPixel(fp + QPointF(-1, -1));
		addPixel(fp + QPointF(1, -1));
		addPixel(fp + QPointF(-1, 1));
		addPixel(fp + QPointF(1, 1));
		
		addPixel(fp + QPointF(0, -1));
		addPixel(fp + QPointF(0, 1));
		addPixel(fp + QPointF(-1, 0));
		addPixel(fp + QPointF(1, 0));
		
		addPixel(fp);
		
		MLArgb result = _argb;
		result.v /= _divisor;
		
		return result;
	}
	
private:
	
	void addPixel(const QPointF &p)
	{
		float dist = mlPointDist(p + QPointF(0.5, 0.5), _p);
		float weight = mlLanczos2WeightF(dist);
		
		_divisor += weight;
		_argb.v += _bitmap->pixel(actualPoint(p.toPoint())).v * weight;
	}
	
	QPoint actualPoint(const QPoint &p)
	{
		QPoint r;
		
		int width = _bitmap->width();
		int height = _bitmap->height();
		
		switch (SpreadType)
		{
		case MLGlobal::SpreadTypePad:
		{
			r.rx() = qBound(0, p.x(), width);
			r.ry() = qBound(0, p.y(), height);
			break;
		}
		case MLGlobal::SpreadTypeRepeat:
		{
			r.rx() = p.x() % width;
			if (r.x() < 0)
				r.rx() += width;
			r.ry() = p.y() % height;
			if (r.y() < 0)
				r.ry() += height;
			break;
		}
		case MLGlobal::SpreadTypeReflective:
		{
			int qx = p.x() / width;
			r.rx() = p.x() % width;
			
			if (r.x() < 0)
			{
				qx--;
				r.rx() += width;
			}
			
			if (qx % 2)
				r.rx() = width - r.x();
			
			int qy = p.y() / height;
			r.ry() = p.y() % height;
			
			if (r.y() < 0)
			{
				qy--;
				r.ry() += height;
			}
			
			if (qy % 2)
				r.ry() = height - r.y();
			
			break;
		}
		}
		
		return r;
	}
	
	void reset()
	{
		_divisor = 0;
		_argb.v = MLSimdF4(0);
	}
	
	void reset(const QPointF &p)
	{
		reset();
		_p = p;
	}

	QPointF _p;
	float _divisor;
	MLArgb _argb;
	MLBitmap<MLArgb> *_bitmap;
};


#endif // MLSCALINGFILTER_H
