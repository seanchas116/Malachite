#include <QtCore>
#include <FreeImage.h>

#include "mlimage.h"
#include "mlimagepaintengine.h"

bool MLImage::isBlank() const
{
	for (int y = 0; y < height(); ++y) {
		const MLArgb *p = constScanline(y);
		for (int x = 0; x < width(); ++x) {
			if (p->a())
				return false;
			p++;
		}
	}
	return true;
}

MLPaintEngine *MLImage::createPaintEngine()
{
	return new MLImagePaintEngine();
}

MLImage MLImage::fromFile(const QString &filePath)
{
	QFileInfo fileInfo(filePath);
	QString extension = fileInfo.fileName().section('.', -1).toLower();
	
	FREE_IMAGE_FORMAT format;
	if (extension == "jpg" || extension == "jpeg") {
		format = FIF_JPEG;
	} else if (extension == "png") {
		format = FIF_PNG;
	} else if (extension == "bmp") {
		format = FIF_BMP;
	} else {
		qWarning() << "MLImage::fromFile: Unknown file type";
		return MLImage();
	}
	
	int flags = 0;
	if (format == FIF_JPEG)
		flags = JPEG_ACCURATE;
	
	FIBITMAP *bitmap = FreeImage_Load(format, filePath.toLocal8Bit(), flags);
	MLImage image = MLImage::fromFIBITMAP(bitmap);
	FreeImage_Unload(bitmap);
	return image;
}

MLImage MLImage::fromFIBITMAP(FIBITMAP *bitmap)
{
	QSize size(FreeImage_GetWidth(bitmap), FreeImage_GetHeight(bitmap));
	MLImage image(size);
	
	FREE_IMAGE_TYPE type = FreeImage_GetImageType(bitmap);
	
	if (type == FIT_BITMAP) {
		int bpp = FreeImage_GetBPP(bitmap);
		if (bpp == 24) {	// RGB8
			qDebug() << "RGB8";
			MLGenericWrapperImageM<MLRgb8> wrapper(FreeImage_GetBits(bitmap), size, FreeImage_GetPitch(bitmap));
			image.paste(wrapper);
		} else if (bpp == 32) {
			qDebug() << "RGBA8";
			MLGenericWrapperImageM<MLArgb8> wrapper(FreeImage_GetBits(bitmap), size, FreeImage_GetPitch(bitmap));
			image.paste(wrapper);
		} else {
			qDebug() << "less than RGBA8";
			FIBITMAP *newBitmap = FreeImage_ConvertTo24Bits(bitmap);
			MLGenericWrapperImageM<MLRgb8> wrapper(FreeImage_GetBits(newBitmap), size, FreeImage_GetPitch(newBitmap));
			image.paste(wrapper);
			FreeImage_Unload(newBitmap);
		}
	} else if (type == FIT_RGB16) {
		qDebug() << "RGB16";
		MLGenericWrapperImageM<MLRgb16> wrapper(FreeImage_GetBits(bitmap), size, FreeImage_GetPitch(bitmap));
		image.paste(wrapper);
	} else if (type == FIT_RGBA16) {
		qDebug() << "RGBA16";
		MLGenericWrapperImageM<MLRgba16> wrapper(FreeImage_GetBits(bitmap), size, FreeImage_GetPitch(bitmap));
		image.paste(wrapper);
	} else {
		qDebug() << "MLImage::fromFIBITMAP: Unsupported data type";
		return MLImage();
	}
	return image;
}


