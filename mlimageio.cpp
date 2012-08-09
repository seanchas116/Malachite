#include <QtGui>
#include "mlimageio.h"
#include "private/mlimageioprivate.h"

#include <FreeImage.h>

MLImageIO::MLImageIO(const QString &filePath, QObject *parent) :
	QObject(parent),
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
	
	if (!_bitmap)
	{
		qDebug() << Q_FUNC_INFO << ": failed to load file";
		return;
	}
	
	_size = QSize(FreeImage_GetWidth(_bitmap), FreeImage_GetHeight(_bitmap));
}

MLImageIO::~MLImageIO()
{
	if (_bitmap) FreeImage_Unload(_bitmap);
}

MLImage MLImageIO::toImage() const
{
	if (!_bitmap)
		return MLImage();
	
	MLImage image(size());
	
	mlPasteFIBITMAPToImage(QPoint(), &image, _bitmap);
	return image;
}

MLSurface MLImageIO::toSurface(const QPoint &p) const
{
	if (!_bitmap)
		return MLSurface();
	
	MLSurface surface;
	mlPasteFIBITMAPToImage(p, &surface, _bitmap);
	return surface;
}

