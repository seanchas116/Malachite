#ifndef MLIMAGEIO_H
#define MLIMAGEIO_H

#include <QDebug>
#include <QObject>
#include "mlsurface.h"
#include "mlgenericwrapperimage.h"
#include <FreeImage.h>

class MALACHITESHARED_EXPORT MLImageIO : public QObject
{
	Q_OBJECT
	
public:
	
	MLImageIO(QObject *parent = 0) :
		QObject(parent), _bitmap(0) {}
	MLImageIO(const QString &filePath, QObject *parent = 0);
	~MLImageIO();
	
	bool isValid() const { return _bitmap; }
	
	FIBITMAP *bitmap() { return _bitmap; }
	uint8_t *bits() { return FreeImage_GetBits(_bitmap); }
	const uint8_t *constBits() const { return FreeImage_GetBits(_bitmap); }
	int bytesPerLine() const { return FreeImage_GetPitch(_bitmap); }
	
	QSize size() const { return _size; }
	
	template <typename Image> bool pasteToImage(const QPoint &p, Image *image) const;
	MLImage toImage() const;
	MLSurface toSurface(const QPoint &p = QPoint()) const;
	
private:
	FIBITMAP *_bitmap;
	QSize _size;
};

template <typename Image>
bool MLImageIO::pasteToImage(const QPoint &p, Image *image) const
{
	FREE_IMAGE_TYPE type = FreeImage_GetImageType(_bitmap);
	
	if (type == FIT_BITMAP) {
		int bpp = FreeImage_GetBPP(_bitmap);
		if (bpp == 24) {	// RGB8
			MLGenericConstWrapperImageM<MLRgb8> wrapper(constBits(), _size, bytesPerLine());
			image->paste(p, wrapper);
		} else if (bpp == 32) {	// ARGB8
			MLGenericConstWrapperImageM<MLArgb8> wrapper(constBits(), _size, bytesPerLine());
			image->paste(p, wrapper);
		} else {
			FIBITMAP *newBitmap = FreeImage_ConvertTo24Bits(_bitmap);
			MLGenericConstWrapperImageM<MLRgb8> wrapper(FreeImage_GetBits(newBitmap), _size, FreeImage_GetPitch(newBitmap));
			image->paste(p, wrapper);
			FreeImage_Unload(newBitmap);
		}
	} else if (type == FIT_RGB16) {
		MLGenericConstWrapperImageM<MLRgb16> wrapper(constBits(), _size, bytesPerLine());
		image->paste(p, wrapper);
	} else if (type == FIT_RGBA16) {
		MLGenericConstWrapperImageM<MLArgb16> wrapper(constBits(), _size, bytesPerLine());
		image->paste(p, wrapper);
	} else {
		qDebug() << "MLImage::fromFIBITMAP: Unsupported data type";
		return false;
	}
	return true;
}

#endif // MLIMAGEIO_H
