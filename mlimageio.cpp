#include <QtGui>
#include "mlimageio.h"

MLImageIO::MLImageIO(const QString &filePath) :
	_bitmap(0)
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
		return;
	}
	
	int flags = 0;
	if (format == FIF_JPEG)
		flags = JPEG_ACCURATE;
	
	_bitmap = FreeImage_Load(format, filePath.toLocal8Bit(), flags);
	_size = QSize(FreeImage_GetWidth(_bitmap), FreeImage_GetHeight(_bitmap));
}

MLImageIO::~MLImageIO()
{
	FreeImage_Unload(_bitmap);
}

MLImage MLImageIO::toImage() const
{
	MLImage image(size());
	pasteToImage(QPoint(), &image);
	return image;
}

MLSurface MLImageIO::toSurface(const QPoint &p) const
{
	MLSurface surface;
	pasteToImage(p, &surface);
	return surface;
}

