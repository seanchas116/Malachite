#ifndef MLGENERICIMAGE_H
#define MLGENERICIMAGE_H

#include <QRect>
#include <QSharedDataPointer>
#include "mlpixelconversion.h"
#include "mlbitmap.h"

template <typename Color>
class MLGenericImageData : public QSharedData
{
public:
	
	typedef Color ColorType;
	
	MLGenericImageData(const QSize &size) :
		_bitmap(0, size),
		_ownsData(true)
	{
		_bitmap.setBits(reinterpret_cast<Color *>(mlAllocateAlignedMemory(_bitmap.byteCount(), 16)));
	}
	
	MLGenericImageData(void *bits, const QSize &size) :
		_bitmap(reinterpret_cast<Color *>(bits), size),
		_ownsData(false)
	{}
	
	MLGenericImageData(const MLGenericImageData &other) :
		QSharedData(other),
		_bitmap(other._bitmap),
		_ownsData(true)
	{
		_bitmap.setBits(reinterpret_cast<Color *>(mlAllocateAlignedMemory(_bitmap.byteCount(), 16)));
		mlCopyArray(_bitmap.bits(), other._bitmap.constBits(), other._bitmap.byteCount());
	}
	
	~MLGenericImageData()
	{
		if (_ownsData)
			mlFreeAlignedMemory(_bitmap.bits());
	}
	
	MLBitmap<Color> _bitmap;
	bool _ownsData;
};

template <ML::ImageFormat Format, class Color>
class MLGenericImage
{
public:
	
	typedef Color ColorType;
	
	MLGenericImage() {}
	MLGenericImage(const QSize &size)
	{
		if (!size.isEmpty())
			p = new MLGenericImageData<Color>(size);
	}
	
	MLGenericImage(int width, int height)
	{
		if (width && height)
			p = new MLGenericImageData<Color>(QSize(width, height));
	}
	
	static MLGenericImage wrap(void *data, const QSize &size)
	{
		MLGenericImage result;
		if (!size.isEmpty())
			result.p = new MLGenericImageData<Color>(data, size);
		
		return result;
	}
	
	static const MLGenericImage wrap(const void *data, const QSize &size)
	{
		MLGenericImage result;
		if (!size.isEmpty())
			result.p = new MLGenericImageData<Color>(const_cast<void *>(data), size);
		
		return result;
	}
	
	QSize size() const { return p ? p->_bitmap.size() : QSize(); }
	QRect rect() const { return QRect(QPoint(), size()); }
	int width() const { return p ? p->_bitmap.width() : 0; }
	int height() const { return p ? p->_bitmap.height() : 0; }
	int bytesPerLine() const { return p ? p->_bitmap.bytesPerLine() : 0; }
	int area() const { return p ? p->_bitmap.area() : 0; }
	
	MLBitmap<Color> bitmap() { return p ? p->_bitmap : MLBitmap<Color>(); }
	const MLBitmap<Color> bitmap() const { return p ? p->_bitmap : MLBitmap<Color>(); }
	
	Color *bits() { return p->_bitmap.bits(); }
	const Color *constBits() const { return p->_bitmap.constBits(); }
	
	Color *scanline(int y) { return p->_bitmap.scanline(y); }
	const Color *constScanline(int y) const { return p->_bitmap.constScanline(y); }
	
	ColorType *pixelPointer(int x, int y) { return p->_bitmap.pixelPointer(x, y); }
	ColorType *pixelPointer(const QPoint &point) { return pixelPointer(point.x(), point.y()); }
	
	const ColorType *constPixelPointer(int x, int y) const { return p->_bitmap.constPixelPointer(x, y); }
	const ColorType *constPixelPointer(const QPoint &point) const { return constPixelPointer(point.x(), point.y()); }
	
	ColorType pixel(int x, int y) const { return *constPixelPointer(x, y); }
	ColorType pixel(const QPoint &point) const { return pixel(point.x(), point.y()); }
	
	void fill(const ColorType &c) { mlFillArray(scanline(0), c, area()); }
	
	template <ML::ImageFormat NewFormat, class NewColor>
	MLGenericImage<NewFormat, NewColor> convert()
	{
		if (!p)
			return MLGenericImage<NewFormat, NewColor>();
		
		MLGenericImage<NewFormat, NewColor> newImage(size());
		
		int count = area();
		NewColor *dp = newImage.bits();
		Color *sp = constBits();
		
		for (int i = 0; i < count; ++i)
			mlConvertPixel<NewFormat, NewColor, Format, Color>(*dp++, *sp++);
	}
	
	template <ML::ImageFormat SrcFormat, class SrcColor>
	void paste(const MLGenericImage<SrcFormat, SrcColor> &image, const QPoint &point = QPoint())
	{
		QRect r = rect() & QRect(point, image.size());
		
		for (int y = r.top(); y <= r.bottom(); ++y)
		{
			ColorType *dp = scanline(y) + r.left();
			const SrcColor *sp = image.constScanline(y - point.y()) + r.left() - point.x();
			
			for (int x = 0; x < r.width(); ++x)
				mlConvertPixel<Format, Color, SrcFormat, SrcColor>(*dp++, *sp++);
		}
	}
	
private:
	QSharedDataPointer<MLGenericImageData<Color> > p;
};


/*
  
  Buffer example
  
template <class Color>
class Buffer
{
public:
	typedef Color ColorType;
	QSize size() const;
	ColorType *scanline();
	const ColorType *constScanline();
};
*/
/*
template <class Buffer>
class MALACHITESHARED_EXPORT MLGenericImage
{
public:
	typedef Buffer BufferType;
	typedef Buffer::ColorType ColorType;
	static ML::ImageFormat format() { return Buffer::format(); }
	static ML::ImageComponentType componentType() { return Buffer::componentType(); }
	
	MLGenericImage(const Buffer &buffer) : buffer(buffer) {}
	
	QSize size() const { return buffer.size(); }
	DataType *scanline(int y) { Q_ASSERT(0 <= y && y < height()); return buffer.scanline(y); }
	const ColorType *constScanline(int y) const { Q_ASSERT(0 <= y && y < height()); return buffer.constScanline(y); }
	DataType *data() { return scanline(0); }
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
}*/


#endif // MLGENERICIMAGE_H
