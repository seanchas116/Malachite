#ifndef MLBRUSH_H
#define MLBRUSH_H

#include <QSharedDataPointer>
#include "mlcolor.h"
#include "mlimage.h"
#include "mlimage.h"
#include "mlsurface.h"
#include <QTransform>

class MLBrushData : public QSharedData
{
public:
	MLBrushData() :
		type(MLGlobal::BrushTypeNull)
	{}
	
	MLBrushData(const MLFastArgbF &argb) :
		type(MLGlobal::BrushTypeColor),
		data(QVariant::fromValue(argb))
	{}
	
	MLBrushData(const MLImage &image) :
		type(MLGlobal::BrushTypeImage),
		data(QVariant::fromValue(image))
	{}
	
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
	
	MLGlobal::BrushType type;
	QVariant data;
	QTransform transform;
};


class MLBrush
{
public:
	
	MLBrush()
	{
		d = new MLBrushData();
	}
	
	MLBrush(const MLColor &color)
	{
		d = new MLBrushData(color.toFastArgbF());
	}
	
	MLBrush(const MLFastArgbF &argb)
	{
		d = new MLBrushData(argb);
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
	MLFastArgbF argb() const { return d->type == MLGlobal::BrushTypeColor ? d->data.value<MLFastArgbF>() : MLFastArgbF(); }
	MLImage image() const { return d->type == MLGlobal::BrushTypeImage ? d->data.value<MLImage>() : MLImage(); }
	MLSurface surface() const { return d->type == MLGlobal::BrushTypeSurface ? d->data.value<MLSurface>() : MLSurface(); }
	
	void setTransform(const QTransform &transform) { d->transform = transform; }
	QTransform transform() const { return d->transform; }
	
private:
	
	QSharedDataPointer<MLBrushData> d;
};

#endif // MLBRUSH_H
