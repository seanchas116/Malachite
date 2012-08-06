#ifndef MLBRUSH_H
#define MLBRUSH_H

#include <QVariant>
#include <QSharedDataPointer>
#include "mlcolor.h"
#include "mlcolorgradient.h"
#include "mlimage.h"
#include "mlsurface.h"
#include <QTransform>
#include <QScopedPointer>

class MLBrushData : public QSharedData
{
public:
	MLBrushData() :
		type(MLGlobal::BrushTypeNull),
		spreadType(MLGlobal::SpreadTypeRepeat)
	{}
	
	MLBrushData(const MLArgb &argb) :
		type(MLGlobal::BrushTypeColor),
		spreadType(MLGlobal::SpreadTypeRepeat),
		data(QVariant::fromValue(argb))
	{}
	
	MLBrushData(const MLImage &image) :
		type(MLGlobal::BrushTypeImage),
		spreadType(MLGlobal::SpreadTypeRepeat),
		data(QVariant::fromValue(image))
	{}
	
	MLBrushData(const MLColorGradient &gradient, const MLLinearGradientInfo &info) :
		type(MLGlobal::BrushTypeLinearGradient),
		spreadType(MLGlobal::SpreadTypeRepeat),
		data(QVariant::fromValue(info)),
		gradient(gradient.clone())
	{}
	
	MLBrushData(const MLColorGradient &gradient, const MLRadialGradientInfo &info) :
		type(MLGlobal::BrushTypeRadialGradient),
		spreadType(MLGlobal::SpreadTypeRepeat),
		data(QVariant::fromValue(info)),
		gradient(gradient.clone())
	{}
	
	MLBrushData(const MLSurface &surface) :
		type(MLGlobal::BrushTypeSurface),
		spreadType(MLGlobal::SpreadTypeRepeat),
		data(QVariant::fromValue(surface))
	{}
	
	MLBrushData(const MLBrushData &other) :
		QSharedData(other),
		type(other.type),
		spreadType(other.spreadType),
		data(other.data),
		transform(other.transform),
		gradient(gradient->clone())
	{}
	
	MLGlobal::BrushType type;
	MLGlobal::SpreadType spreadType;
	QVariant data;
	QTransform transform;
	QScopedPointer<MLColorGradient> gradient;
};


class MALACHITESHARED_EXPORT MLBrush
{
public:
	
	MLBrush() {}
	
	MLBrush(const MLColor &color)
	{
		d = new MLBrushData(color.toArgb());
	}
	
	MLBrush(const MLArgb &argb)
	{
		d = new MLBrushData(argb);
	}
	
	MLBrush(const MLColorGradient &gradient, const MLLinearGradientInfo &info)
	{
		d = new MLBrushData(gradient, info);
	}
	
	MLBrush(const MLColorGradient &gradient, const MLRadialGradientInfo &info)
	{
		d = new MLBrushData(gradient, info);
	}
	
	MLBrush(const MLImage &image)
	{
		d = new MLBrushData(image);
	}
	
	MLBrush(const MLSurface &surface)
	{
		d = new MLBrushData(surface);
	}
	
	static MLBrush fromLinearGradient(const MLColorGradient &gradient, const QPointF &start, const QPointF &end)
	{
		return MLBrush(gradient, MLLinearGradientInfo(start, end));
	}
	
	static MLBrush fromRadialGradient(const MLColorGradient &gradient, const QPointF &center, double radius, const QPointF &focal)
	{
		return MLBrush(gradient, MLRadialGradientInfo(center, radius, focal));
	}
	
	static MLBrush fromRadialGradient(const MLColorGradient &gradient, const QPointF &center, double radius)
	{
		return MLBrush(gradient, MLRadialGradientInfo(center, radius));
	}
	
	MLGlobal::BrushType type() const { return d->type; }
	
	void setSpreadType(MLGlobal::SpreadType type) { d->spreadType = type; }
	MLGlobal::SpreadType spreadType() const { return d->spreadType; }
	
	MLArgb argb() const { return d->type == MLGlobal::BrushTypeColor ? d->data.value<MLArgb>() : MLArgb(); }
	MLImage image() const { return d->type == MLGlobal::BrushTypeImage ? d->data.value<MLImage>() : MLImage(); }
	MLSurface surface() const { return d->type == MLGlobal::BrushTypeSurface ? d->data.value<MLSurface>() : MLSurface(); }
	
	MLLinearGradientInfo linearGradientInfo() const { return d->type == MLGlobal::BrushTypeLinearGradient ? d->data.value<MLLinearGradientInfo>() : MLLinearGradientInfo(); }
	MLRadialGradientInfo radialGradientInfo() const { return d->type == MLGlobal::BrushTypeRadialGradient ? d->data.value<MLRadialGradientInfo>() : MLRadialGradientInfo(); }
	
	const MLColorGradient *gradient() const { return d->gradient.data(); }
	
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
