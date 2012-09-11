#ifndef GENERICIMAGE_H
#define GENERICIMAGE_H

#include <QRect>
#include <QSharedDataPointer>
#include "mlpixelconversion.h"
#include "mlbitmap.h"

namespace Malachite
{

template <typename T_Color>
class GenericImageData : public QSharedData
{
public:
	
	typedef T_Color ColorType;
	
	GenericImageData(const QSize &size, int bytesPerLine) :
		bitmap(Pointer<T_Color>(), size, bytesPerLine),
		ownsData(true)
	{
		bitmap.setBits(allocateAlignedMemory(bitmap.byteCount(), 16), bitmap.byteCount());
	}
	
	GenericImageData(void *bits, const QSize &size, int bytesPerLine) :
		bitmap(Pointer<T_Color>(), size, bytesPerLine),
		ownsData(false)
	{
		bitmap.setBits(bits, bitmap.byteCount());
	}
	
	GenericImageData(const GenericImageData &other) :
		QSharedData(other),
		bitmap(other.bitmap),
		ownsData(true)
	{
		bitmap.setBits(allocateAlignedMemory(bitmap.byteCount(), 16), bitmap.byteCount());
		bitmap.bits().pasteByte(other.bitmap.constBits(), other.bitmap.byteCount());
	}
	
	~GenericImageData()
	{
		if (ownsData)
			freeAlignedMemory(bitmap.bits());
	}
	
	Bitmap<T_Color> bitmap;
	const bool ownsData;
};

template <Malachite::ImageFormat T_Format, class T_Color>
class GenericImage
{
public:
	
	typedef T_Color ColorType;
	
	GenericImage() {}
	GenericImage(const QSize &size, int bytesPerLine)
	{
		if (!size.isEmpty())
			p = new GenericImageData<T_Color>(size, bytesPerLine);
	}
	
	GenericImage(const QSize &size) : GenericImage(size, size.width() * sizeof(T_Color)) {}
	GenericImage(int width, int height, int bytesPerLine) : GenericImage(QSize(width, height), bytesPerLine) {}
	GenericImage(int width, int height) : GenericImage(QSize(width, height), width * sizeof(T_Color)) {}
	
	static GenericImage wrap(void *data, const QSize &size, int bytesPerLine)
	{
		GenericImage r;
		if (!size.isEmpty())
			r.p = new GenericImageData<T_Color>(data, size, bytesPerLine);
		return r;
	}
	
	static GenericImage wrap(void *data, const QSize &size) { return wrap(data, size, size.width() * sizeof(T_Color)); }
	
	static const GenericImage wrap(const void *data, const QSize &size, int bytesPerLine)
	{
		return wrap(const_cast<void *>(data), size, bytesPerLine);
	}
	
	static const GenericImage wrap(const void *data, const QSize &size) { wrap(data, size, size.width() * sizeof(T_Color)); }
	
	void detach() { p.detach(); }
	bool isValid() const { return p; }
	
	QSize size() const { return p ? p->bitmap.size() : QSize(); }
	QRect rect() const { return QRect(QPoint(), size()); }
	int width() const { return p ? p->bitmap.width() : 0; }
	int height() const { return p ? p->bitmap.height() : 0; }
	int bytesPerLine() const { return p ? p->bitmap.bytesPerLine() : 0; }
	int area() const { return p ? p->bitmap.area() : 0; }
	
	Bitmap<T_Color> bitmap() { return p ? p->bitmap : Bitmap<T_Color>(); }
	const Bitmap<T_Color> constBitmap() const { return p ? p->bitmap : Bitmap<T_Color>(); }
	
	Pointer<T_Color> bits() { return p->bitmap.bits(); }
	const T_Color *constBits() const { return p->bitmap.constBits(); }
	
	Pointer<T_Color> scanline(int y) { return p->bitmap.scanline(y); }
	Pointer<const T_Color> constScanline(int y) const { return p->bitmap.constScanline(y); }
	
	Pointer<T_Color> invertedScanline(int invertedY) { return p->bitmap.invertedScanline(invertedY); }
	Pointer<const T_Color> invertedConstScanline(int invertedY) const { return p->bitmap.invertedConstScanline(invertedY); }
	
	Pointer<T_Color> pixelPointer(int x, int y) { return p->bitmap.pixelPointer(x, y); }
	Pointer<T_Color> pixelPointer(const QPoint &point) { return pixelPointer(point.x(), point.y()); }
	
	Pointer<const T_Color> constPixelPointer(int x, int y) const { return p->bitmap.constPixelPointer(x, y); }
	Pointer<const T_Color> constPixelPointer(const QPoint &point) const { return constPixelPointer(point.x(), point.y()); }
	
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
	
	template <Malachite::ImageFormat NewFormat, class NewColor>
	GenericImage<NewFormat, NewColor> convert()
	{
		if (!p)
			return GenericImage<NewFormat, NewColor>();
		
		QSize s = size();
		GenericImage<NewFormat, NewColor> newImage(s);
		
		for (int y = 0; y < s.height(); ++y)
		{
			NewColor *dp = newImage.scanline(y);
			T_Color *sp = constScanline(y);
			
			for (int x = 0; x < s.width(); ++x)
				convertPixel<NewFormat, NewColor, T_Format, T_Color>(*dp++, *sp++);
		}
	}
	
	template <bool DstInverted = false, bool SrcInverted = false, Malachite::ImageFormat SrcFormat, class SrcColor>
	void paste(const GenericImage<SrcFormat, SrcColor> &image, const QPoint &point = QPoint())
	{
		QRect r = rect() & QRect(point, image.size());
		
		for (int y = r.top(); y <= r.bottom(); ++y)
		{
			ColorType *dp = (DstInverted ? invertedScanline(y) : scanline(y)) + r.left();
			const SrcColor *sp = (SrcInverted ? image.invertedConstScanline(y - point.y()) : image.constScanline(y - point.y())) + r.left() - point.x();
			
			for (int x = 0; x < r.width(); ++x)
				convertPixel<T_Format, T_Color, SrcFormat, SrcColor>(*dp++, *sp++);
		}
	}
	
	bool operator==(const GenericImage<T_Format, T_Color> &other) const
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
			if (memcmp(p->bitmap.constScanline(y), other.p->bitmap.constScanline(y), size.width() * sizeof(T_Color)))
				return false;
		}
		
		return true;
	}
	
private:
	QSharedDataPointer<GenericImageData<T_Color> > p;
};

}

#endif // GENERICIMAGE_H
