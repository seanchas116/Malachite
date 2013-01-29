#include <QtCore>
#include <FreeImage.h>

#include "painter.h"
#include "private/imagepaintengine.h"
#include "image.h"

namespace Malachite
{

bool Image::isBlank() const
{
	/*
	for (int y = 0; y < height(); ++y) {
		const MLArgb *p = constScanline(y);
		for (int x = 0; x < width(); ++x) {
			if (p->a())
				return false;
			p++;
		}
	}
	return true;*/
	
	int count = area();
	const Pixel *p = constBits();
	
	for (int i = 0; i < count; ++i)
	{
		if (p->a()) return false;
		p++;
	}
	
	return true;
}

Image Image::toOpaqueImage() const
{
	Image image = *this;
	
	Painter painter(&image);
	painter.setColor(Color::white());
	painter.setBlendMode(BlendMode::DestinationOver);
	painter.drawRect(image.rect());
	
	painter.end();
	
	return image;
}

PaintEngine *Image::createPaintEngine()
{
	return new ImagePaintEngine;
}

QImage Image::toQImage() const
{
	QImage qimage(size(), QImage::Format_ARGB32_Premultiplied);
	
	auto wrapper = GenericImage<BgraPremultU8>::wrap(qimage.bits(), qimage.size(), qimage.bytesPerLine());
	wrapper.paste(*this);
	return qimage;
}

Image Image::fromQImage(const QImage &qimage)
{
	Image image(qimage.size());
	
	auto wrapper = GenericImage<BgraPremultU8>::wrap(qimage.bits(), qimage.size(), qimage.bytesPerLine());
	image.paste(wrapper);
	return image;
}

QByteArray Image::toByteArray() const
{
	QByteArray data;
	QDataStream stream(&data, QIODevice::WriteOnly);
	
	int count = width() * height();
	data.reserve(count * BytesPerPixel);
	
	Pointer<const Pixel> p = constBits();
	
	for (int i = 0; i < count; ++i)
	{
		stream << p->a();
		stream << p->r();
		stream << p->g();
		stream << p->b();
		p++;
	}
	
	return data;
}

Image Image::fromByteArray(const QByteArray &data, const QSize &size)
{
	int count = size.width() * size.height();
	if (data.size() < count * BytesPerPixel)
		return Image();
	
	Image image(size);
	
	Pointer<Pixel> p = image.bits();
	
	QDataStream stream(data);
	
	for (int i = 0; i < count; ++i)
	{
		stream >> p->ra();
		stream >> p->rr();
		stream >> p->rg();
		stream >> p->rb();
		p++;
	}
	
	return image;
}

Image &Image::operator*=(float factor)
{
	factor = qBound(0.f, factor, 1.f);
	if (factor == 1.f)
		return *this;
	
	PixelVec vfactor(factor);
	QSize size = this->size();
	
	for (int y = 0; y < size.height(); ++y)
	{
		Pixel *p = scanline(y);
		
		for (int x = 0; x < size.width(); ++x)
			(p++)->rv() *= vfactor;
	}
	
	return *this;
}

}
