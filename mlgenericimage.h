#ifndef MLGENERICIMAGE_H
#define MLGENERICIMAGE_H

#include <QRect>
#include "mlglobal.h"

/*
  
  Buffer example
  
template <class Color>
class Buffer
{
public:
	typedef Color ColorType
	QSize size() const;
	ColorType *scanline();
	const ColorType *constScanline();
};
*/

template <class Buffer>
class MALACHITESHARED_EXPORT MLGenericImage
{
public:
	typedef Buffer BufferType;
	typedef typename Buffer::ColorType ColorType;
	
	MLGenericImage(const Buffer &buffer) : buffer(buffer) {}
	
	QSize size() const { return buffer.size(); }
	ColorType *scanline(int y) { Q_ASSERT(0 <= y && y < height()); return buffer.scanline(y); }
	const ColorType *constScanline(int y) const { Q_ASSERT(0 <= y && y < height()); return buffer.constScanline(y); }
	ColorType *data() { return scanline(0); }
	const ColorType *constData() const { return constScanline(0); }
	
	QRect rect() const { return QRect(QPoint(), size()); }
	int width() const { return size().width(); }
	int height() const { return size().height(); }
	int pixelCount() const { return width() * height(); }
	
	ColorType *pixelPointer(int x, int y)
	{
		ColorType *p = scanline(y);
		Q_ASSERT(p);
		Q_ASSERT(0 <= x && x < size().width());
		return p + x;
	}
	
	ColorType *pixelPointer(const QPoint &point) { return pixelPointer(point.x(), point.y()); }
	
	const ColorType *constPixelPointer(int x, int y) const
	{
		const ColorType *p = constScanline(y);
		Q_ASSERT(p);
		Q_ASSERT(0 <= x && x < size().width());
		return p + x;
	}
	
	const ColorType *constPixelPointer(const QPoint &point) const { return constPixelPointer(point.x(), point.y()); }
	
	ColorType pixel(int x, int y) const { return *constPixelPointer(x, y); }
	ColorType pixel(const QPoint &point) const { return pixel(point.x(), point.y()); }
	
	ColorType pixelReflected(int x, int y) const
	{
		if (x < 0)
			x = -x - 1;
		else if (x >= width())
			x = 2 * width() - x - 1;
		if (y < 0)
			y = -y - 1;
		else if (y >= height())
			y = 2 * height() - y - 1;
		
		return pixel(x, y);
	}
	
	ColorType pixelReflected(const QPoint &point) const { return pixelReflected(point.x(), point.y()); }
	
	void fill(const ColorType &c)
	{
		int w = width();
		int h = height();
		if (0 < w && 0 < h)
		{
			for (int i = 0; i < h; ++i)
			{
				mlFillArray(scanline(i), c, w);
			}
		}
	}
	
	template <class OtherImage> void paste(const QPoint &point, const OtherImage &image)
	{
		typedef typename OtherImage::ColorType OtherColorType;
		
		QRect r = rect() & QRect(point, image.size());
		
		for (int y = r.top(); y <= r.bottom(); ++y)
		{
			ColorType *dp = scanline(y) + r.left();
			const OtherColorType *sp = image.constScanline(y - point.y()) + r.left() - point.x();
			
			mlCopyColor(r.width(), dp, sp);
		}
	}
	
	template <class OtherImage> void paste(int x, int y, const OtherImage &image) { paste(QPoint(x, y), image); }
	template <class OtherImage> void paste(const OtherImage &image) { paste(QPoint(), image); }
	
protected:
	
	Buffer buffer;
};

template <class ColorDst, class ColorSrc>
void mlCopyColor(int count, ColorDst *dst, const ColorSrc *src)
{
	for (int i = 0; i < count; ++i) *dst++ = *src++;
}


#endif // MLGENERICIMAGE_H
