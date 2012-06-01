#ifndef MLBITMAP_H
#define MLBITMAP_H

#include <QSize>
#include "mlmisc.h"

template <typename Color>
class MLBitmap
{
public:
	MLBitmap() :
		_bits(0)
	{}
	
	MLBitmap(uint8_t *bits, const QSize &size, int bytesPerLine) :
		_bits(bits),
		_size(size),
		_bytesPerLine(bytesPerLine)
	{}
	
	static MLBitmap allocate(const QSize &size, int bytesPerLine)
	{
		return MLBitmap(reinterpret_cast<uint8_t *>(mlAllocateAlignedMemory(size.height() * bytesPerLine, 16)), size, bytesPerLine);
	}
	
	uint8_t *bits() { return _bits; }
	const uint8_t *constBits() const { return _bits; }
	QSize size() const { return _size; }
	int bytesPerLine() const { return _bytesPerLine; }
	int byteCount() const { return _size.height() * _bytesPerLine; }
	QRect rect() const { return QRect(QPoint(), _size); }
	
	Color *scanline(int y)
	{
		return reinterpret_cast<Color *>(_bits + _bytesPerLine * y);
	}
	
	const Color *constScanline(int y) const
	{
		return reinterpret_cast<const Color *>(_bits + _bytesPerLine * y);
	}
	
	Color *pixelPointer(int x, int y)
	{
		return reinterpret_cast<Color *>(_bits + _bytesPerLine * y + sizeof(Color) * x);
	}
	
	Color *pixelPointer(const QPoint &p) { return pixelPointer(p.x(), p.y()); }
	
	const Color *constPixelPointer(int x, int y) const
	{
		return reinterpret_cast<const Color *>(_bits + _bytesPerLine * y + sizeof(Color) * x);
	}
	
	const Color *constPixelPointer(const QPoint &p) const { return constPixelPointer((p.x(), p.y())); }
	
private:
	uint8_t *_bits;
	QSize _size;
	int _bytesPerLine;
};

#endif // MLBITMAP_H
