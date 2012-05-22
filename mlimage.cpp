#include <QtCore>

#include "mlimage.h"
#include "mlimagepaintengine.h"

MLPaintEngine *MLImage::createPaintEngine()
{
	return new MLImagePaintEngine();
}

/*
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
			image.paste(0, 0, wrapper);
		} else if (bpp == 32) {
			MLGenericWrapperImageM<MLRgba8> wrapper(FreeImage_GetBits(bitmap), size, FreeImage_GetPitch(bitmap));
			image.paste(0, 0, wrapper);
		} else {
			FIBITMAP *newBitmap = FreeImage_ConvertTo24Bits(bitmap);
			MLGenericWrapperImageM<MLRgb8> wrapper(FreeImage_GetBits(newBitmap), size, FreeImage_GetPitch(newBitmap));
			image.paste(0, 0, wrapper);
			FreeImage_Unload(newBitmap);
		}
	} else if (type == FIT_RGB16) {
		MLGenericWrapperImageM<MLRgb16> wrapper(FreeImage_GetBits(bitmap), size, FreeImage_GetPitch(bitmap));
		image.paste(0, 0, wrapper);
	} else if (type == FIT_RGBA16) {
		MLGenericWrapperImageM<MLRgba16> wrapper(FreeImage_GetBits(bitmap), size, FreeImage_GetPitch(bitmap));
		image.paste(0, 0, wrapper);
	} else {
		qDebug() << "MLImage::fromFIBITMAP: Unsupported data type";
		return MLImage();
	}
	return image;
}*/


