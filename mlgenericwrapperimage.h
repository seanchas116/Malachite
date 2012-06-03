#ifndef MLGENERICWRAPPERIMAGE_H
#define MLGENERICWRAPPERIMAGE_H

#include "mlbitmap.h"
#include "mlgenericimage.h"

template <typename Color>
class MLWrapperImageBuffer
{
public:
	typedef Color ColorType;
	
	MLWrapperImageBuffer(const MLBitmap<Color> &bitmap) :
		_bitmap(bitmap) {}
	
	MLWrapperImageBuffer(uint8_t *bits, const QSize &size, int bytesPerLine) :
		_bitmap(bits, size, bytesPerLine) {}
	
	QSize size() const { return _bitmap.size(); }
	Color *scanline(int y) { return _bitmap.scanline(y); }
	const Color *constScanline(int y) const { return _bitmap.constScanline(y); }
	
private:
	MLBitmap<Color> _bitmap;
};

template <typename Color>
class MLConstWrapperImageBuffer
{
public:
	typedef Color ColorType;
	
	MLConstWrapperImageBuffer(const MLConstBitmap<Color> &bitmap) :
		_bitmap(bitmap) {}
	
	MLConstWrapperImageBuffer(const uint8_t *bits, const QSize &size, int bytesPerLine) :
		_bitmap(bits, size, bytesPerLine) {}
	
	QSize size() const { return _bitmap.size(); }
	const Color *constScanline(int y) const { return _bitmap.constScanline(y); }
	
private:
	MLConstBitmap<Color> _bitmap;
};

// Vertically inverted
template <typename Color>
class MLWrapperImageMBuffer
{
public:
	typedef Color ColorType;
	
	MLWrapperImageMBuffer(const MLBitmap<Color> &bitmap) :
		_bitmap(bitmap) {}
	
	MLWrapperImageMBuffer(uint8_t *bits, const QSize &size, int bytesPerLine) :
		_bitmap(bits, size, bytesPerLine) {}
	
	QSize size() const { return _bitmap.size(); }
	Color *scanline(int y) { return _bitmap.scanline(size().height() - y - 1); }
	const Color *constScanline(int y) const { return _bitmap.constScanline(size().height() - y - 1); }
	
private:
	MLBitmap<Color> _bitmap;
};

template <typename Color>
class MLConstWrapperImageMBuffer
{
public:
	typedef Color ColorType;
	
	MLConstWrapperImageMBuffer(const MLConstBitmap<Color> &bitmap) :
		_bitmap(bitmap) {}
	
	MLConstWrapperImageMBuffer(const uint8_t *bits, const QSize &size, int bytesPerLine) :
		_bitmap(bits, size, bytesPerLine) {}
	
	QSize size() const { return _bitmap.size(); }
	const Color *constScanline(int y) const { return _bitmap.constScanline(size().height() - y - 1); }
	
private:
	MLConstBitmap<Color> _bitmap;
};


template <class Color>
class MLGenericWrapperImage : public MLGenericImage<MLWrapperImageBuffer<Color> >
{
public:
	MLGenericWrapperImage(const MLBitmap<Color> &bitmap) :
		MLGenericImage<MLWrapperImageBuffer<Color> >(MLWrapperImageBuffer<Color>(bitmap)) {}
	
	MLGenericWrapperImage(uint8_t *bits, const QSize &size, int bytesPerLine) :
		MLGenericImage<MLWrapperImageBuffer<Color> >(MLWrapperImageBuffer<Color>(bits, size, bytesPerLine)) {}
	
	MLGenericWrapperImage(uint8_t *bits, int width, int height, int bytesPerLine) :
		MLGenericImage<MLWrapperImageBuffer<Color> >(MLWrapperImageBuffer<Color>(bits, QSize(width, height), bytesPerLine)) {}
	
	MLBitmap<Color> bitmap() { return this->buffer._bitmap; }
	const MLBitmap<Color> constBitmap() const { return this->buffer._bitmap; }
};

template <class Color>
class MLGenericConstWrapperImage : public MLGenericImage<MLConstWrapperImageBuffer<Color> >
{
public:
	MLGenericConstWrapperImage(const MLConstBitmap<Color> &bitmap) :
		MLGenericImage<MLConstWrapperImageBuffer<Color> >(MLConstWrapperImageBuffer<Color>(bitmap)) {}
	
	MLGenericConstWrapperImage(const uint8_t *bits, const QSize &size, int bytesPerLine) :
		MLGenericImage<MLConstWrapperImageBuffer<Color> >(MLConstWrapperImageBuffer<Color>(bits, size, bytesPerLine)) {}
	
	MLGenericConstWrapperImage(const uint8_t *bits, int width, int height, int bytesPerLine) :
		MLGenericImage<MLConstWrapperImageBuffer<Color> >(MLConstWrapperImageBuffer<Color>(bits, QSize(width, height), bytesPerLine)) {}
	
	MLConstBitmap<Color> bitmap() { return this->buffer._bitmap; }
	const MLConstBitmap<Color> constBitmap() const { return this->buffer._bitmap; }
};

template <class Color>
class MLGenericWrapperImageM : public MLGenericImage<MLWrapperImageMBuffer<Color> >
{
public:
	MLGenericWrapperImageM(const MLBitmap<Color> &bitmap) :
		MLGenericImage<MLWrapperImageMBuffer<Color> >(MLWrapperImageMBuffer<Color>(bitmap)) {}
	
	MLGenericWrapperImageM(uint8_t *bits, const QSize &size, int bytesPerLine) :
		MLGenericImage<MLWrapperImageMBuffer<Color> >(MLWrapperImageMBuffer<Color>(bits, size, bytesPerLine)) {}
	
	MLGenericWrapperImageM(uint8_t *bits, int width, int height, int bytesPerLine) :
		MLGenericImage<MLWrapperImageMBuffer<Color> >(MLWrapperImageMBuffer<Color>(bits, QSize(width, height), bytesPerLine)) {}
	
	MLBitmap<Color> bitmap() { return this->buffer._bitmap; }
	const MLBitmap<Color> constBitmap() const { return this->buffer._bitmap; }
};

template <class Color>
class MLGenericConstWrapperImageM : public MLGenericImage<MLConstWrapperImageMBuffer<Color> >
{
public:
	MLGenericConstWrapperImageM(const MLConstBitmap<Color> &bitmap) :
		MLGenericImage<MLConstWrapperImageMBuffer<Color> >(MLConstWrapperImageMBuffer<Color>(bitmap)) {}
	
	MLGenericConstWrapperImageM(const uint8_t *bits, const QSize &size, int bytesPerLine) :
		MLGenericImage<MLConstWrapperImageMBuffer<Color> >(MLConstWrapperImageMBuffer<Color>(bits, size, bytesPerLine)) {}
	
	MLGenericConstWrapperImageM(const uint8_t *bits, int width, int height, int bytesPerLine) :
		MLGenericImage<MLConstWrapperImageMBuffer<Color> >(MLConstWrapperImageMBuffer<Color>(bits, QSize(width, height), bytesPerLine)) {}
	
	MLConstBitmap<Color> bitmap() { return this->buffer._bitmap; }
	const MLConstBitmap<Color> constBitmap() const { return this->buffer._bitmap; }
};

#endif // MLGENERICWRAPPERIMAGE_H
