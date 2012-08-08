#ifndef MLBITMAP_H
#define MLBITMAP_H

#include <QSize>
#include <QRect>
#include "mlmisc.h"

template <typename Color>
class MLBitmap
{
public:
	MLBitmap() :
		_bits(0)
	{}
	
	MLBitmap(Color *bits, const QSize &size) :
		_bits(bits),
		_size(size)
	{}
	
	void setBits(Color *bits) { _bits = bits; }
	Color *bits() { return _bits; }
	const Color *constBits() const { return _bits; }
	QSize size() const { return _size; }
	int width() const { return _size.width(); }
	int height() const { return _size.height(); }
	int byteCount() const { return area() * sizeof(Color); }
	int area() const { return _size.height() * _size.width(); }
	QRect rect() const { return QRect(QPoint(), _size); }
	
	Color *scanline(int y) { return _bits + _size.width() * y; }
	const Color *constScanline(int y) const { return _bits + _size.width() * y; }
	
	Color *pixelPointer(int x, int y) { return _bits + _size.width() * y + x; }
	Color *pixelPointer(const QPoint &p) { return pixelPointer(p.x(), p.y()); }
	
	const Color *constPixelPointer(int x, int y) const { return _bits + _size.width() * y + x; }
	const Color *constPixelPointer(const QPoint &p) const { return constPixelPointer((p.x(), p.y())); }
	
	Color pixel(int x, int y) const { return *constPixelPointer(x, y); }
	Color pixel(const QPoint &p) const { return pixel(p.x(), p.y()); }
	
private:
	Color *_bits;
	QSize _size;
};

#endif // MLBITMAP_H
