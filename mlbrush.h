#ifndef MLBRUSH_H
#define MLBRUSH_H

#include <QVariant>
#include <QSharedDataPointer>
#include "mlcolor.h"
#include "mlcolorgradient.h"
#include "mlimage.h"
#include "mlsurface.h"
#include <QTransform>

class MLBrushData : public QSharedData
{
public:
	MLBrushData() :
		type(MLGlobal::BrushTypeNull)
	{}
	
	MLBrushData(const MLArgb &argb) :
		type(MLGlobal::BrushTypeColor),
		data(QVariant::fromValue(argb))
	{}
	
	MLBrushData(const MLImage &image) :
		type(MLGlobal::BrushTypeImage),
		data(QVariant::fromValue(image))
	{}
	
	MLBrushData(const MLColorGradient &gradient) :
		type(MLGlobal::BrushTypeGradient),
		data(QVariant::fromValue(gradient))
	{
		cache = new MLColorGradientCache;
		cache->load(gradient);
	}
	
	MLBrushData(const MLSurface &surface) :
		type(MLGlobal::BrushTypeSurface),
		data(QVariant::fromValue(surface))
	{}
	
	MLBrushData(const MLBrushData &other) :
		QSharedData(other),
		type(other.type),
		data(other.data),
		transform(other.transform)
	{}
	
	~MLBrushData()
	{
		if (type == MLGlobal::BrushTypeGradient)
			delete cache;
	}
	
	MLGlobal::BrushType type;
	QVariant data;
	MLColorGradientCache *cache;
	QTransform transform;
};


class MALACHITESHARED_EXPORT MLBrush
{
public:
	
	MLBrush()
	{
		d = new MLBrushData();
	}
	
	MLBrush(const MLColor &color)
	{
		d = new MLBrushData(color.toArgb());
	}
	
	MLBrush(const MLArgb &argb)
	{
		d = new MLBrushData(argb);
	}
	
	MLBrush(const MLColorGradient &gradient)
	{
		d = new MLBrushData(gradient);
	}
	
	MLBrush(const MLImage &image)
	{
		d = new MLBrushData(image);
	}
	
	MLBrush(const MLSurface &surface)
	{
		d = new MLBrushData(surface);
	}
	
	MLGlobal::BrushType type() const { return d->type; }
	
	MLArgb argb() const { return d->type == MLGlobal::BrushTypeColor ? d->data.value<MLArgb>() : MLArgb(); }
	MLImage image() const { return d->type == MLGlobal::BrushTypeImage ? d->data.value<MLImage>() : MLImage(); }
	MLSurface surface() const { return d->type == MLGlobal::BrushTypeSurface ? d->data.value<MLSurface>() : MLSurface(); }
	MLColorGradient gradient() const { return d->type == MLGlobal::BrushTypeGradient ? d->data.value<MLColorGradient>() : MLColorGradient(); }
	
	void setTransform(const QTransform &transform) { d->transform = transform; }
	QTransform transform() const { return d->transform; }
	
	void translate(double dx, double dy) { d->transform.translate(dx, dy); }
	void translate(const QPointF &d) { translate(d.x(), d.y()); }
	void translate(const QPoint &d) { translate(d.x(), d.y()); }
	void scale(double sx, double sy) { d->transform.scale(sx, sy); }
	void rotate(double angle) { d->transform.rotate(angle); }
	
private:
	
	QSharedDataPointer<MLBrushData> d;
};

Q_DECLARE_METATYPE(MLBrush)

#endif // MLBRUSH_H
