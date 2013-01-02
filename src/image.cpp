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
	const Vec4F *p = constBits();
	
	for (int i = 0; i < count; ++i)
	{
		if (p->a) return false;
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

/*
MLArgb MLImage::colorSummation()
{
	int count = pixelCount();
	const MLArgb *p = constData();
	
	
}
*/

#define COLOR_SUM_MAX 1024

/*
static MLVec4F mlColorSummation(int count, const MLVec4F *data)
{
	MLVec4F r(0);
	
	for (int i = 0; i < count; ++i)
	{
		r += *data++;
	}
	
	return r;
}*/

static Vec4F mlColorSummation(int count, Pointer<const Vec4F> data, Pointer<const Vec4F> mask)
{
	Vec4F r(0);
	
	for (int i = 0; i < count; ++i)
	{
		r += *data * mask->a;
		data++;
		mask++;
	}
	
	return r;
}

Vec4F Image::colorSummation(const QPoint &maskOffset, const Image &mask) const
{
	QRect dstRect = QRect(QPoint(), size());
	QRect srcRect = QRect(maskOffset, mask.size());
	
	QRect targetRect = dstRect & srcRect;
	
	if (targetRect.isEmpty())
		return Vec4F(0);
	
	int blockCount = targetRect.width() * targetRect.height() / COLOR_SUM_MAX + 1;
	QScopedArrayPointer<Vec4F> blocks(new Vec4F[blockCount]);
	
	Vec4F remainderArgb;
	int remainder = 0;
	int currentBlock = 0;
	
	for (int y = targetRect.top(); y <= targetRect.bottom(); ++y)
	{
		QPoint p(targetRect.left(), y);
		
		Pointer<const Vec4F> pointer = constPixelPointer(p);
		Pointer<const Vec4F> maskPointer = mask.constPixelPointer(p - maskOffset);
		
		int rx = targetRect.width();
		int x = 0;
		blocks[currentBlock] = remainderArgb + mlColorSummation(remainder, pointer, maskPointer);
		currentBlock++;
		rx -= remainder;
		
		while (rx >= COLOR_SUM_MAX)
		{
			x = targetRect.width() - rx;
			blocks[currentBlock] = mlColorSummation(COLOR_SUM_MAX, pointer + x, maskPointer + x);
			currentBlock++;
			rx -= COLOR_SUM_MAX;
		}
		
		x = targetRect.width() - rx;
		remainderArgb = mlColorSummation(rx, pointer + x, maskPointer + x);
		remainder = COLOR_SUM_MAX - rx;
	}
	
	blocks[currentBlock] = remainderArgb;
	
	Vec4F r(0);
	
	for (int i = 0; i < blockCount; ++i)
	{
		r += blocks[i];
	}
	
	return r;
}

PaintEngine *Image::createPaintEngine()
{
	return new ImagePaintEngine;
}

QImage Image::toQImage() const
{
	QImage image(size(), QImage::Format_ARGB32_Premultiplied);
	
	auto wrapper = MLImage32::wrap(image.bits(), image.size(), image.bytesPerLine());
	wrapper.paste(*this);
	return image;
}

Image Image::fromQImage(const QImage &qimage)
{
	Image image(qimage.size());
	
	auto wrapper = MLImage32::wrap(qimage.constBits(), qimage.size(), qimage.bytesPerLine());
	image.paste(wrapper);
	return image;
}

QByteArray Image::toByteArray() const
{
	QByteArray data;
	QDataStream stream(&data, QIODevice::WriteOnly);
	
	int count = width() * height();
	data.reserve(count * BytesPerPixel);
	
	Pointer<const Vec4F> p = constBits();
	
	for (int i = 0; i < count; ++i)
	{
		stream << p->a;
		stream << p->r;
		stream << p->g;
		stream << p->b;
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
	
	Pointer<Vec4F> p = image.bits();
	
	QDataStream stream(data);
	
	for (int i = 0; i < count; ++i)
	{
		stream >> p->a;
		stream >> p->r;
		stream >> p->g;
		stream >> p->b;
		p++;
	}
	
	return image;
}

Image &Image::operator*=(float factor)
{
	factor = qBound(0.f, factor, 1.f);
	if (factor == 1.f)
		return *this;
	
	Vec4F vfactor(factor);
	QSize size = this->size();
	
	for (int y = 0; y < size.height(); ++y)
	{
		Vec4F *p = scanline(y);
		
		for (int x = 0; x < size.width(); ++x)
			*p++ *= vfactor;
	}
	
	return *this;
}

}
