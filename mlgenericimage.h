#ifndef MLGENERICIMAGE_H
#define MLGENERICIMAGE_H

#include <QDebug>
#include <QSharedDataPointer>
#include <QRect>
#include "mlmisc.h"

template <typename Color>
class MLBitmap
{
public:
	MLBitmap(uint8_t *bits, const QSize &size, int bytesPerLine);
	
	static MLBitmap allocate(const QSize &size, int bytesPerLine) :
		_bits(reinterpret_cast<uint8_t *>(size.height() * bytesPerLine)),
		_size(size),
		_bytesPerLine(bytesPerLine)
	{}
	
	uint8_t *bits() { return _bits; }
	const uint8_t *constBits() const { return _bits; }
	QSize size() const { return _size; }
	int bytesPerLine() const { return _bytesPerLine; }
	int byteCount() const { return _size.height() * _bytesPerLine; }
	
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

template <typename Color>
class MALACHITESHARED_EXPORT MLAbstractImage
{
public:
	typedef Color ColorType;
	virtual QSize size() const = 0;
	virtual Color *scanline(int y) = 0;
	virtual const Color *constScanline(int y) const = 0;
	
	int width() const { return size().width(); }
	int height() const { return size().height(); }
	int pixelCount() const { return size().width() * size().height(); }
	
	Color *pixelPointer(int x, int y) {
		Color *p = scanline(y);
		Q_ASSERT(p);
		Q_ASSERT(0 <= y && y < size().height());
		Q_ASSERT(0 <= x && x < size().width());
		return p + x;
	}
	
	Color *pixelPointer(const QPoint &point) { return pixelPointer(point.x(), point.y()); }
	
	const Color *constPixelPointer(int x, int y) const {
		const Color *p = constScanline(y);
		Q_ASSERT(p);
		Q_ASSERT(0 <= y && y < size().height());
		Q_ASSERT(0 <= x && x < size().width());
		return p + x;
	}
	
	const Color *constPixelPointer(const QPoint &point) const { return constPixelPointer(point.x(), point.y()); }
	
	Color pixel(int x, int y) const {
		const Color *p = constPixelPointer(x, y);
		Q_ASSERT(p);
		return *p;
	}
	
	Color pixel(const QPoint &point) const { return pixel(point.x(), point.y()); }
	
	void fill(const Color &c)
	{
		int w = width();
		int h = height();
		if (0 < w && 0 < h) {
			for (int i = 0; i < h; ++i) {
				mlFillArray(scanline(i), c, w);
			}
		}
	}
};

template <typename Color>
class MLGenericWrapperImage : public MLAbstractImage<Color>
{
public:
	
	MLGenericWrapperImage(const MLBitmap<Color> &bitmap) :
		_bitmap(bitmap) {}
	
	QSize size() const { return _bitmap.size(); }
	
	Color *scanline(int y)
	{
		Q_ASSERT(0 <= y && y < _size.height());
		return _bitmap.scanline(y);
	}
	
	const Color *constScanline(int y) const
	{
		Q_ASSERT(0 <= y && y < _size.height());
		return _bitmap.constScanline(y);
	}
	
private:
	MLBitmap<Color> _bitmap;
};

// Vertically inverted
template <typename Color>
class MLGenericWrapperImageM : public MLAbstractImage<Color>
{
public:
	MLGenericWrapperImageM(const MLBitmap<Color> &bitmap) :
		_bitmap(bitmap) {}
	
	QSize size() const { return _bitmap.size(); }
	
	Color *scanline(int y)
	{
		Q_ASSERT(0 <= y && y < _size.height());
		return _bitmap.scanline(size().height() - y - 1);
	}
	
	const Color *constScanline(int y) const
	{
		Q_ASSERT(0 <= y && y < _size.height());
		return _bitmap.constScanline(size().height() - y - 1);
	}
	
private:
	MLBitmap<Color> _bitmap;
};

template <typename Color>
class MLGenericImageData : public QSharedData
{
public:
	MLGenericImageData(const QSize &size, int bytesPerLine) :
		_bitmap(MLBitmap<Color>::allocate(size, bytesPerLine))
	{}
	
	MLGenericImageData(const MLGenericImageData &other) :
		_bitmap(MLBitmap<Color>::allocate(size, bytesPerLine))
	{
		mlCopyArray(_bitmap.bits(), other.constBits(), byteCount());
	}
	
	MLGenericImageData(const QSize &size, int bytesPerLine)
	    : _size(size),
		  _bytesPerLine(bytesPerLine)
	{
		_bits = reinterpret_cast<uint8_t *>(mlAllocateAlignedMemory(bytesPerLine * size.height(), 16));
	}
	
	MLGenericImageData(const MLGenericImageData &other)
	    : QSharedData(other),
	      _size(other._size),
		  _bytesPerLine(other._bytesPerLine)
	{
		_bits = reinterpret_cast<uint8_t *>(mlAllocateAlignedMemory(_bytesPerLine * _size.height(), 16));
		mlCopyArray(_bits, other._bits, byteCount());
	}
	
	~MLGenericImageData() {
		mlFreeAlignedMemory(_bits);
	}
	
	int byteCount() const { return _bitmap.byteCount(); }
	QSize size() const { return _bitmap.size(); }
	int bytesPerLine() const { return _bitmap.bytesPerLine(); }
	uint8_t *bits() { return _bitmap.bits(); }
	const uint8_t *constBits() const { return _bitmap.constBits(); }
	
private:
	MLBitmap<Color> _bitmap;
};

template <typename Color>
class MALACHITESHARED_EXPORT MLGenericImage : public MLAbstractImage<Color>
{
public:
	MLGenericImage() {}
	
	MLGenericImage(const QSize &size) {
		if (!size.isEmpty())
			d = new MLGenericImageData(size, size.width() * sizeof(Color));
	}
	
	MLGenericImage(const QSize &size, int bytesPerLine) {
		if (!size.isEmpty())
			d = new MLGenericImageData(size, bytesPerLine);
	}
	
	MLGenericImage(int width, int height) {
		if (width > 0 && height > 0)
			d = new MLGenericImageData(QSize(width, height), width * sizeof(Color));
	}
	
	MLGenericImage(int width, int height, int bytesPerLine) {
		if (width > 0 && height > 0)
			d = new MLGenericImageData(QSize(width, height), bytesPerLine);
	}
	
	MLGenericImage(const MLGenericImage<Color> &other) :
		MLAbstractImage<Color>(),
		d(other.d) {}
	
	void detach() {
		if (!d) return;
		d.detach();
	}
	
	int byteCount() const {
		if (!d) return 0;
		return d->byteCount();
	}
	
	int bytesPerLine() const {
		if (!d) return 0;
		return d->bytesPerLine();
	}
	
	uint8_t *bits() {
		if (!d) return 0;
		return d->bits();
	}
	
	const uint8_t *constBits() const {
		if (!d) return 0;
		return d->constBits();
	}
	
	MLBitmap<Color> bitmap() { return d->_bitmap; }
	const MLBitmap<Color> constBitmap() const { return d->_bitmap; }
	
	bool isValid() const { return d; }
	
	QSize size() const {
		if (!d) return QSize(0, 0);
		return d->size();
	}
	
	Color *scanline(int y)
	{
		Q_ASSERT(0 <= y && y < size().height());
		return reinterpret_cast<Color *>(d->bits() + y * d->bytesPerLine());
	}
	
	const Color *constScanline(int y) const
	{
		Q_ASSERT(0 <= y && y < size().height());
		return reinterpret_cast<const Color *>(d->constBits() + y * d->bytesPerLine());
	}
	
private:
	QSharedDataPointer<MLGenericImageData> d;
};

/*
// paint the destination image with the source image using the pixel operation function (such as source-over, multiply, etc)
template <typename ColorDest, typename ColorSrc>
void mlGenericImageOperation(MLAbstractImage<ColorDest> *dstImage, const QPoint &dstPosition, const MLAbstractImage<ColorSrc> *srcImage, const QPoint &srcPosition, void (*pixelOperation)(int count, ColorDest *d, const ColorSrc *s))
{
	// either image is invalid
	if (dstImage->size().isEmpty() || srcImage->size().isEmpty())
		return;
	
	QRect dstRect = QRect(dstPosition, dstImage->size());
	QRect srcRect = QRect(srcPosition, srcImage->size());
	
	QRect targetRect = dstRect & srcRect;
	if (!targetRect.isValid())
		return;
	
	for (int y = 0; y < targetRect.height(); ++y) {
		ColorDest *dp = dstImage->pixelPointer(targetRect.topLeft() - dstRect.topLeft() + QPoint(0, y));
		const ColorSrc *sp = srcImage->constPixelPointer(targetRect.topLeft() - srcRect.topLeft() + QPoint(0, y));
		pixelOperation(targetRect.width(), dp, sp);
	}
}
*?



#endif // MLGENERICIMAGE_H
