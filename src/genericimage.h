#ifndef GENERICIMAGE_H
#define GENERICIMAGE_H

//ExportName: GenericImage

#include <QRect>
#include <QSharedDataPointer>
#include "pixelconversion.h"
#include "bitmap.h"

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

enum ImagePasteInversionMode
{
	ImagePasteNotInverted = 0,
	ImagePasteDestinationInverted = 1,
	ImagePasteSourceInverted = 2,
	ImagePasteBothInverted = ImagePasteDestinationInverted | ImagePasteSourceInverted
};

template <class T_Color>
class MALACHITESHARED_EXPORT GenericImage
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
	
	QSize size() const
		{ return p ? p->bitmap.size() : QSize(); }
	QRect rect() const
		{ return QRect(QPoint(), size()); }
	int width() const
		{ return p ? p->bitmap.width() : 0; }
	int height() const
		{ return p ? p->bitmap.height() : 0; }
	int bytesPerLine() const
		{ return p ? p->bitmap.bytesPerLine() : 0; }
	int area() const
		{ return p ? p->bitmap.area() : 0; }
	
	Bitmap<T_Color> bitmap() { return p ? p->bitmap : Bitmap<T_Color>(); }
	const Bitmap<T_Color> constBitmap() const { return p ? p->bitmap : Bitmap<T_Color>(); }
	
	Pointer<T_Color> bits()
		{ Q_ASSERT(p); return p->bitmap.bits(); }
	Pointer<const T_Color> constBits() const
		{ Q_ASSERT(p); return p->bitmap.constBits(); }
	
	Pointer<T_Color> scanline(int y)
		{ Q_ASSERT(p); return p->bitmap.scanline(y); }
	Pointer<const T_Color> constScanline(int y) const
		{ Q_ASSERT(p); return p->bitmap.constScanline(y); }
	
	Pointer<T_Color> invertedScanline(int invertedY)
		{ Q_ASSERT(p); return p->bitmap.invertedScanline(invertedY); }
	Pointer<const T_Color> invertedConstScanline(int invertedY) const
		{ Q_ASSERT(p); return p->bitmap.invertedConstScanline(invertedY); }
	
	Pointer<T_Color> pixelPointer(int x, int y)
		{ Q_ASSERT(p); return p->bitmap.pixelPointer(x, y); }
	Pointer<T_Color> pixelPointer(const QPoint &point)
		{ return pixelPointer(point.x(), point.y()); }
	
	Pointer<const T_Color> constPixelPointer(int x, int y) const
		{ Q_ASSERT(p); return p->bitmap.constPixelPointer(x, y); }
	Pointer<const T_Color> constPixelPointer(const QPoint &point) const
		{ return constPixelPointer(point.x(), point.y()); }
	
	ColorType pixel(int x, int y) const { return *constPixelPointer(x, y); }
	ColorType pixel(const QPoint &point) const { return pixel(point.x(), point.y()); }
	
	void setPixel(int x, int y, const ColorType &color)
	{
		ColorType *p = pixelPointer(x, y);
		Q_ASSERT(p);
		*p = color;
	}
	
	void setPixel(const QPoint &p, const ColorType &color) { setPixel(p.x(), p.y(), color); }
	
	void fill(const ColorType &c)
	{
		Q_ASSERT(p);
		
		QSize s = size();
		for (int y = 0; y < s.height(); ++y)
		{
			scanline(y).fill(c, s.width());
		}
	}
	
	template <class NewColor>
	GenericImage<NewColor> convert()
	{
		if (!p)
			return GenericImage<NewColor>();
		
		QSize s = size();
		GenericImage<NewColor> newImage(s);
		
		for (int y = 0; y < s.height(); ++y)
		{
			NewColor *dp = newImage.scanline(y);
			T_Color *sp = constScanline(y);
			
			for (int x = 0; x < s.width(); ++x)
				*dp++ = *sp++;
		}
	}
	
	template <ImagePasteInversionMode InversionMode = ImagePasteNotInverted, class SrcColor>
	void paste(const GenericImage<SrcColor> &image, const QPoint &point = QPoint())
	{
		QRect r = rect() & QRect(point, image.size());
		
		for (int y = r.top(); y <= r.bottom(); ++y)
		{
			ColorType *dp;
			
			if (InversionMode & ImagePasteDestinationInverted)
				dp = invertedScanline(y);
			else
				dp = scanline(y);
			
			dp += r.left();
			
			const SrcColor *sp;
			
			if (InversionMode & ImagePasteSourceInverted)
				sp = image.invertedConstScanline(y - point.y());
			else
				sp = image.constScanline(y - point.y());
			
			sp += (r.left() - point.x());
			
			for (int x = 0; x < r.width(); ++x)
				*dp++ = *sp++;
		}
	}
	
	bool operator==(const GenericImage &other) const
	{
		if (p == other.p)
			return true;
		
		if (isValid() != other.isValid())
			return false;
		
		Q_ASSERT(p);
		
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
	
	bool operator!=(const GenericImage &other) const
	{
		return !(*this == other);
	}
	
	bool referenceIsEqualTo(const GenericImage &other) const
	{
		return p == other.p;
	}
	
private:
	QSharedDataPointer<GenericImageData<T_Color> > p;
};

}

#endif // GENERICIMAGE_H
