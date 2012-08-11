#include <QtCore>
#include <FreeImage.h>

#include "private/mlimagepaintengine.h"
#include "mlimage.h"

bool MLImage::isBlank() const
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
	const MLVec4F *p = constBits();
	
	for (int i = 0; i < count; ++i)
	{
		if (p->a) return false;
		p++;
	}
	
	return true;
}

/*
MLArgb MLImage::colorSummation()
{
	int count = pixelCount();
	const MLArgb *p = constData();
	
	
}
*/

#define COLOR_SUM_MAX 1024

static MLVec4F mlColorSummation(int count, const MLVec4F *data)
{
	MLVec4F r(0);
	
	for (int i = 0; i < count; ++i)
	{
		r += *data++;
	}
	
	return r;
}

static MLVec4F mlColorSummation(int count, MLPointer<const MLVec4F> data, MLPointer<const MLVec4F> mask)
{
	MLVec4F r(0);
	
	for (int i = 0; i < count; ++i)
	{
		r += *data * mask->a;
		data++;
		mask++;
	}
	
	return r;
}

MLVec4F MLImage::colorSummation(const QPoint &maskOffset, const MLImage &mask) const
{
	QRect dstRect = QRect(QPoint(), size());
	QRect srcRect = QRect(maskOffset, mask.size());
	
	QRect targetRect = dstRect & srcRect;
	
	if (targetRect.isEmpty())
		return MLVec4F(0);
	
	int blockCount = targetRect.width() * targetRect.height() / COLOR_SUM_MAX + 1;
	QScopedArrayPointer<MLVec4F> blocks(new MLVec4F[blockCount]);
	
	MLVec4F remainderArgb;
	int remainder = 0;
	int currentBlock = 0;
	
	for (int y = targetRect.top(); y <= targetRect.bottom(); ++y)
	{
		QPoint p(targetRect.left(), y);
		
		MLPointer<const MLVec4F> pointer = constPixelPointer(p);
		MLPointer<const MLVec4F> maskPointer = mask.constPixelPointer(p - maskOffset);
		
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
	
	MLVec4F r(0);
	
	for (int i = 0; i < blockCount; ++i)
	{
		r += blocks[i];
	}
	
	return r;
}

MLPaintEngine *MLImage::createPaintEngine()
{
	return new MLImagePaintEngine;
}

QImage MLImage::toQImage() const
{
	QImage image(size(), QImage::Format_ARGB32_Premultiplied);
	
	auto wrapper = MLImage32::wrap(image.bits(), image.size(), image.bytesPerLine());
	wrapper.paste(*this);
	return image;
}

MLImage MLImage::fromQImage(const QImage &qimage)
{
	MLImage image(qimage.size());
	
	auto wrapper = MLImage32::wrap(qimage.constBits(), qimage.size(), qimage.bytesPerLine());
	image.paste(wrapper);
	return image;
}


