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
		type(MLGlobal::BrushTypeNull),
		spreadType(MLGlobal::SpreadTypeRepeat),
		cache(0)
	{}
	
	MLBrushData(const MLArgb &argb) :
		type(MLGlobal::BrushTypeColor),
		spreadType(MLGlobal::SpreadTypeRepeat),
		data(QVariant::fromValue(argb)),
		cache(0)
	{}
	
	MLBrushData(const MLImage &image) :
		type(MLGlobal::BrushTypeImage),
		spreadType(MLGlobal::SpreadTypeRepeat),
		data(QVariant::fromValue(image)),
		cache(0)
	{}
	
	MLBrushData(const MLLinearColorGradient &linearGradient) :
		type(MLGlobal::BrushTypeLinearGradient),
		spreadType(MLGlobal::SpreadTypeRepeat),
		data(QVariant::fromValue(linearGradient))
	{
		cache = new MLColorGradientCache();
		cache->load(linearGradient);
	}
	
	MLBrushData(const MLRadialColorGradient &radialGradient) :
		type(MLGlobal::BrushTypeRadialGradient),
		spreadType(MLGlobal::SpreadTypeRepeat),
		data(QVariant::fromValue(radialGradient))
	{
		cache = new MLColorGradientCache();
		cache->load(radialGradient);
	}
	
	MLBrushData(const MLSurface &surface) :
		type(MLGlobal::BrushTypeSurface),
		spreadType(MLGlobal::SpreadTypeRepeat),
		data(QVariant::fromValue(surface)),
		cache(0)
	{}
	
	MLBrushData(const MLBrushData &other) :
		QSharedData(other),
		type(other.type),
		spreadType(other.spreadType),
		data(other.data),
		transform(other.transform)
	{
		cache = other.cache ? new MLColorGradientCache(*(other.cache)) : 0;
	}
	
	~MLBrushData()
	{
		if (cache)
			delete cache;
	}
	
	MLGlobal::BrushType type;
	MLGlobal::SpreadType spreadType;
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
	
	MLBrush(const MLLinearColorGradient &linearGradient)
	{
		d = new MLBrushData(linearGradient);
	}
	
	MLBrush(const MLRadialColorGradient &radialGradient)
	{
		d = new MLBrushData(radialGradient);
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
	MLGlobal::SpreadType spreadType() const { return d->spreadType; }
	
	MLArgb argb() const { return d->type == MLGlobal::BrushTypeColor ? d->data.value<MLArgb>() : MLArgb(); }
	MLImage image() const { return d->type == MLGlobal::BrushTypeImage ? d->data.value<MLImage>() : MLImage(); }
	MLSurface surface() const { return d->type == MLGlobal::BrushTypeSurface ? d->data.value<MLSurface>() : MLSurface(); }
	MLLinearColorGradient linearGradient() const { return d->type == MLGlobal::BrushTypeLinearGradient ? d->data.value<MLLinearColorGradient>() : MLLinearColorGradient(); }
	MLRadialColorGradient radialGradient() const { return d->type == MLGlobal::BrushTypeRadialGradient ? d->data.value<MLRadialColorGradient>() : MLRadialColorGradient(); }
	
	const MLColorGradientCache *gradientCache() { return d->cache; }
	
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
