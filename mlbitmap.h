#ifndef MLBITMAP_H
#define MLBITMAP_H

#include <QSize>
#include <QRect>
#include "mlmisc.h"
#include "mlmemory.h"

template <typename Color>
class MLBitmap
{
public:
	MLBitmap() :
		_bits(MLPointer<Color>())
	{}
	
	MLBitmap(MLPointer<Color> bits, const QSize &size, int bytesPerLine) :
		_bits(bits),
		_size(size),
		_bytesPerLine(bytesPerLine)
	{}
	
	void setBits(MLPointer<Color> *bits) { _bits = bits; }
	void setBits(void *data, int byteCount) { _bits = MLPointer<Color>(reinterpret_cast<Color *>(data), byteCount); }
	
	MLPointer<Color> bits() { return _bits; }
	const MLPointer<const Color> constBits() const { return _bits; }
	QSize size() const { return _size; }
	int width() const { return _size.width(); }
	int height() const { return _size.height(); }
	int byteCount() const { return _bytesPerLine * _size.height(); }
	int bytesPerLine() const { return _bytesPerLine; }
	int area() const { return _size.height() * _size.width(); }
	QRect rect() const { return QRect(QPoint(), _size); }
	
	MLPointer<Color> scanline(int y) { return _bits.byteOffset(_bytesPerLine * y); }
	MLPointer<const Color> constScanline(int y) const { return _bits.byteOffset(_bytesPerLine * y); }
	
	MLPointer<Color> invertedScanline(int invertedY) { return scanline(height() - invertedY - 1); }
	MLPointer<const Color> invertedConstScanline(int invertedY) const { return constScanline(height() - invertedY - 1); }
	
	MLPointer<Color> pixelPointer(int x, int y) { return scanline(y) + x; }
	MLPointer<Color> pixelPointer(const QPoint &p) { return pixelPointer(p.x(), p.y()); }
	
	MLPointer<const Color> constPixelPointer(int x, int y) const { return constScanline(y) + x; }
	MLPointer<const Color> constPixelPointer(const QPoint &p) const { return constPixelPointer((p.x(), p.y())); }
	
	Color pixel(int x, int y) const { return *constPixelPointer(x, y); }
	Color pixel(const QPoint &p) const { return pixel(p.x(), p.y()); }
	
private:
	MLPointer<Color> _bits;
	QSize _size;
	int _bytesPerLine;
};

#endif // MLBITMAP_H
