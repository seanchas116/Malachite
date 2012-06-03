#ifndef MLGENERICSHAREDIMAGE_H
#define MLGENERICSHAREDIMAGE_H

#include <QSharedData>
#include "mlbitmap.h"
#include "mlgenericimage.h"

template <typename Color>
class MLSharedImageBufferData : public QSharedData
{
public:
	
	typedef Color ColorType;
	
	MLSharedImageBufferData(const QSize &size, int bytesPerLine) :
		_bitmap(0, size, bytesPerLine)
	{
		_bitmap.setBits(reinterpret_cast<uint8_t *>(mlAllocateAlignedMemory(size.height() * bytesPerLine, 16)));
	}
	
	MLSharedImageBufferData(const MLSharedImageBufferData &other) :
		QSharedData(other),
		_bitmap(other._bitmap)
	{
		_bitmap.setBits(reinterpret_cast<uint8_t *>(mlAllocateAlignedMemory(_bitmap.size().height() * _bitmap.bytesPerLine(), 16)));
		mlCopyArray(_bitmap.bits(), other._bitmap.constBits(), other._bitmap.byteCount());
	}
	
	~MLSharedImageBufferData()
	{
		mlFreeAlignedMemory(_bitmap.bits());
	}
	
	MLBitmap<Color> _bitmap;
};

template <typename Color>
class MLSharedImageBuffer
{
public:
	
	typedef Color ColorType;
	
	MLSharedImageBuffer() {}
	
	MLSharedImageBuffer(const QSize &size, int bytesPerLine)
	{
		if (!size.isEmpty())
			d = new MLSharedImageBufferData<Color>(size, bytesPerLine);
	}
	
	uint8_t *bits() { return d->_bitmap.bits(); }
	const uint8_t *constBits() const { return d->_bitmap.constBits(); }
	
	QSize size() const { return d ? d->_bitmap.size() : QSize(0, 0); }
	
	Color *scanline(int y)
	{
		Q_ASSERT(0 <= y && y < size().height());
		return reinterpret_cast<Color *>(d->_bitmap.bits() + y * d->_bitmap.bytesPerLine());
	}
	
	const Color *constScanline(int y) const
	{
		Q_ASSERT(0 <= y && y < size().height());
		return reinterpret_cast<const Color *>(d->_bitmap.constBits() + y * d->_bitmap.bytesPerLine());
	}
	
	QSharedDataPointer<MLSharedImageBufferData<Color> > d;
};

template <class Color>
class MLGenericSharedImage : public MLGenericImage<MLSharedImageBuffer<Color> >
{
public:
	
	MLGenericSharedImage() :
		MLGenericImage<MLSharedImageBuffer<Color> > (MLSharedImageBuffer<Color>()) {}
	
	MLGenericSharedImage(const QSize &size, int bytesPerLine) :
		MLGenericImage<MLSharedImageBuffer<Color> >(MLSharedImageBuffer<Color>(size, bytesPerLine)) {}
	
	MLGenericSharedImage(int width, int height, int bytesPerLine) :
		MLGenericImage<MLSharedImageBuffer<Color> >(MLSharedImageBuffer<Color>(QSize(width, height), bytesPerLine)) {}
	
	MLGenericSharedImage(const QSize &size) :
		MLGenericImage<MLSharedImageBuffer<Color> >(MLSharedImageBuffer<Color>(size, size.width() * sizeof(Color))) {}
	
	MLGenericSharedImage(int width, int height) :
		MLGenericImage<MLSharedImageBuffer<Color> >(MLSharedImageBuffer<Color>(QSize(width, height), width * sizeof(Color))) {}
	
	void detach() { if (this->buffer.d) this->buffer.d.detach(); }
	
	MLBitmap<Color> bitmap() { return this->buffer.d->_bitmap; }
	const MLBitmap<Color> constBitmap() const { return this->buffer.d->_bitmap; }
	
	bool isValid() const { return this->buffer.d; }
};

#endif // MLGENERICSHAREDIMAGE_H
