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
	
	MLGenericImageData(const QSize &size, int bytesPerLine) :
		bitmap(MLPointer<Color>(), size, bytesPerLine),
		ownsData(true)
	{
		bitmap.setBits(mlAllocateAlignedMemory(bitmap.byteCount(), 16), bitmap.byteCount());
	}
	
	MLGenericImageData(void *bits, const QSize &size, int bytesPerLine) :
		bitmap(MLPointer<Color>(), size, bytesPerLine),
		ownsData(false)
	{
		bitmap.setBits(bits, bitmap.byteCount());
	}
	
	MLGenericImageData(const MLGenericImageData &other) :
		QSharedData(other),
		bitmap(other.bitmap),
		ownsData(true)
	{
		bitmap.setBits(mlAllocateAlignedMemory(bitmap.byteCount(), 16), bitmap.byteCount());
		bitmap.bits().pasteByte(other.bitmap.constBits(), other.bitmap.byteCount());
	}
	
	~MLGenericImageData()
	{
		if (ownsData)
			mlFreeAlignedMemory(bitmap.bits());
	}
	
	MLBitmap<Color> bitmap;
	const bool ownsData;
};

template <ML::ImageFormat Format, class Color>
class MLGenericImage
{
public:
	
	typedef Color ColorType;
	
	MLGenericImage() {}
	MLGenericImage(const QSize &size, int bytesPerLine)
	{
		if (!size.isEmpty())
			p = new MLGenericImageData<Color>(size, bytesPerLine);
	}
	
	MLGenericImage(const QSize &size) : MLGenericImage(size, size.width() * sizeof(Color)) {}
	MLGenericImage(int width, int height, int bytesPerLine) : MLGenericImage(QSize(width, height), bytesPerLine) {}
	MLGenericImage(int width, int height) : MLGenericImage(QSize(width, height), width * sizeof(Color)) {}
	
	static MLGenericImage wrap(void *data, const QSize &size, int bytesPerLine)
	{
		MLGenericImage r;
		if (!size.isEmpty())
			r.p = new MLGenericImageData<Color>(data, size, bytesPerLine);
		return r;
	}
	
	static MLGenericImage wrap(void *data, const QSize &size) { return wrap(data, size, size.width() * sizeof(Color)); }
	
	static const MLGenericImage wrap(const void *data, const QSize &size, int bytesPerLine)
	{
		return wrap(const_cast<void *>(data), size, bytesPerLine);
	}
	
	static const MLGenericImage wrap(const void *data, const QSize &size) { wrap(data, size, size.width() * sizeof(Color)); }
	
	void detach() { p.detach(); }
	bool isValid() const { return p; }
	
	QSize size() const { return p ? p->bitmap.size() : QSize(); }
	QRect rect() const { return QRect(QPoint(), size()); }
	int width() const { return p ? p->bitmap.width() : 0; }
	int height() const { return p ? p->bitmap.height() : 0; }
	int bytesPerLine() const { return p ? p->bitmap.bytesPerLine() : 0; }
	int area() const { return p ? p->bitmap.area() : 0; }
	
	MLBitmap<Color> bitmap() { return p ? p->bitmap : MLBitmap<Color>(); }
	const MLBitmap<Color> constBitmap() const { return p ? p->bitmap : MLBitmap<Color>(); }
	
	MLPointer<Color> bits() { return p->bitmap.bits(); }
	const Color *constBits() const { return p->bitmap.constBits(); }
	
	MLPointer<Color> scanline(int y) { return p->bitmap.scanline(y); }
	MLPointer<const Color> constScanline(int y) const { return p->bitmap.constScanline(y); }
	
	MLPointer<Color> invertedScanline(int invertedY) { return p->bitmap.invertedScanline(invertedY); }
	MLPointer<const Color> invertedConstScanline(int invertedY) const { return p->bitmap.invertedConstScanline(invertedY); }
	
	MLPointer<Color> pixelPointer(int x, int y) { return p->bitmap.pixelPointer(x, y); }
	MLPointer<Color> pixelPointer(const QPoint &point) { return pixelPointer(point.x(), point.y()); }
	
	MLPointer<const Color> constPixelPointer(int x, int y) const { return p->bitmap.constPixelPointer(x, y); }
	MLPointer<const Color> constPixelPointer(const QPoint &point) const { return constPixelPointer(point.x(), point.y()); }
	
	ColorType pixel(int x, int y) const { return *constPixelPointer(x, y); }
	ColorType pixel(const QPoint &point) const { return pixel(point.x(), point.y()); }
	
	void fill(const ColorType &c)
	{
		if (!p) return;
		
		QSize s = size();
		for (int y = 0; y < s.height(); ++y)
		{
			scanline(y).fill(c, s.width());
		}
	}
	
	template <ML::ImageFormat NewFormat, class NewColor>
	MLGenericImage<NewFormat, NewColor> convert()
	{
		if (!p)
			return MLGenericImage<NewFormat, NewColor>();
		
		QSize s = size();
		MLGenericImage<NewFormat, NewColor> newImage(s);
		
		for (int y = 0; y < s.height(); ++y)
		{
			NewColor *dp = newImage.scanline(y);
			Color *sp = constScanline(y);
			
			for (int x = 0; x < s.width(); ++x)
				mlConvertPixel<NewFormat, NewColor, Format, Color>(*dp++, *sp++);
		}
	}
	
	template <bool DstInverted = false, bool SrcInverted = false, ML::ImageFormat SrcFormat, class SrcColor>
	void paste(const MLGenericImage<SrcFormat, SrcColor> &image, const QPoint &point = QPoint())
	{
		QRect r = rect() & QRect(point, image.size());
		
		for (int y = r.top(); y <= r.bottom(); ++y)
		{
			ColorType *dp = (DstInverted ? invertedScanline(y) : scanline(y)) + r.left();
			const SrcColor *sp = (SrcInverted ? image.invertedConstScanline(y - point.y()) : image.constScanline(y - point.y())) + r.left() - point.x();
			
			for (int x = 0; x < r.width(); ++x)
				mlConvertPixel<Format, Color, SrcFormat, SrcColor>(*dp++, *sp++);
		}
	}
	
	bool operator==(const MLGenericImage<Format, Color> &other) const
	{
		if (p == other.p)
			return true;
		
		if (isValid() != other.isValid())
			return false;
		
		if (!isValid() && other.isValid())
			return false;
		
		QSize size = p->bitmap.size();
		
		if (size != other.p->bitmap.size())
			return false;
		
		for (int y = 0; y < size.height(); ++y)
		{
			if (memcmp(p->bitmap.constScanline(y), other.p->bitmap.constScanline(y), size.width() * sizeof(Color)))
				return false;
		}
		
		return true;
	}
	
private:
	QSharedDataPointer<MLGenericImageData<Color> > p;
};

#endif // MLGENERICIMAGE_H
