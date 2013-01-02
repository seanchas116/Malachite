#ifndef Bitmap_H
#define Bitmap_H

//ExportName: Bitmap

#include <QSize>
#include <QRect>
#include "misc.h"
#include "memory.h"

namespace Malachite
{

template <typename T_Color>
class MALACHITESHARED_EXPORT Bitmap
{
public:
	Bitmap() :
		_bits(Pointer<T_Color>())
	{}
	
	Bitmap(Pointer<T_Color> bits, const QSize &size, int bytesPerLine) :
		_bits(bits),
		_size(size),
		_bytesPerLine(bytesPerLine)
	{}
	
	void setBits(Pointer<T_Color> *bits) { _bits = bits; }
	void setBits(void *data, int byteCount) { _bits = Pointer<T_Color>(reinterpret_cast<T_Color *>(data), byteCount); }
	
	Pointer<T_Color> bits() { return _bits; }
	const Pointer<const T_Color> constBits() const { return _bits; }
	QSize size() const { return _size; }
	int width() const { return _size.width(); }
	int height() const { return _size.height(); }
	int byteCount() const { return _bytesPerLine * _size.height(); }
	int bytesPerLine() const { return _bytesPerLine; }
	int area() const { return _size.height() * _size.width(); }
	QRect rect() const { return QRect(QPoint(), _size); }
	
	Pointer<T_Color> scanline(int y)
	{
		Q_ASSERT(0 <= y && y < _size.height());
		return _bits.byteOffset(_bytesPerLine * y);
	}
	Pointer<const T_Color> constScanline(int y) const
	{
		Q_ASSERT(0 <= y && y < _size.height());
		return _bits.byteOffset(_bytesPerLine * y);
	}
	
	Pointer<T_Color> invertedScanline(int invertedY) { return scanline(height() - invertedY - 1); }
	Pointer<const T_Color> invertedConstScanline(int invertedY) const { return constScanline(height() - invertedY - 1); }
	
	Pointer<T_Color> pixelPointer(int x, int y)
	{
		Q_ASSERT(0 <= x && x < _size.width());
		return scanline(y) + x;
	}
	Pointer<T_Color> pixelPointer(const QPoint &p) { return pixelPointer(p.x(), p.y()); }
	
	Pointer<const T_Color> constPixelPointer(int x, int y) const
	{
		Q_ASSERT(0 <= x && x < _size.width());
		return constScanline(y) + x;
	}
	Pointer<const T_Color> constPixelPointer(const QPoint &p) const { return constPixelPointer((p.x(), p.y())); }
	
	T_Color pixel(int x, int y) const { return *constPixelPointer(x, y); }
	T_Color pixel(const QPoint &p) const { return pixel(p.x(), p.y()); }
	
private:
	Pointer<T_Color> _bits;
	QSize _size;
	int _bytesPerLine;
};

}

#endif // Bitmap_H
