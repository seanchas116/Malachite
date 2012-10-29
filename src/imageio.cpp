#include <QtGui>
#include "imageio.h"
#include "private/imageioprivate.h"

#include <FreeImage.h>

namespace Malachite
{

ImageImporter::ImageImporter(const QString &filePath) :
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

ImageImporter::~ImageImporter()
{
	if (_bitmap) FreeImage_Unload(_bitmap);
}

Image ImageImporter::toImage() const
{
	if (!_bitmap)
		return Image();
	
	Image image(size());
	
	pasteFIBITMAPToImage(QPoint(), &image, _bitmap);
	return image;
}

Surface ImageImporter::toSurface(const QPoint &p) const
{
	if (!_bitmap)
		return Surface();
	
	Surface surface;
	pasteFIBITMAPToImage(p, &surface, _bitmap);
	return surface;
}

ImageExporter::ImageExporter(const QSize &size, const QString &format) :
	_size(size),
	_format(format)
{
	if (format == "png")
		_bitmap = FreeImage_AllocateT(FIT_RGBA16, size.width(), size.height());
	else if (format == "jpg")
		_bitmap = FreeImage_Allocate(size.width(), size.height(), 24);
	else if (format == "bmp")
		_bitmap = FreeImage_Allocate(size.width(), size.height(), 24);
	else
	{
		_bitmap = 0;
		qWarning() << Q_FUNC_INFO << ": unsupported format";
	}
}

ImageExporter::ImageExporter(const Surface &surface, const QSize &size, const QString &format) :
	ImageExporter(size, format)
{
	pasteSurface(surface, QPoint());
}

ImageExporter::ImageExporter(const Image &image, const QString &format) :
	ImageExporter(image.size(), format)
{
	pasteImage(image, QPoint());
}

ImageExporter::~ImageExporter()
{
	if (_bitmap)
		FreeImage_Unload(_bitmap);
}

bool ImageExporter::save(const QString &filePath, int quality)
{
	if (!_bitmap)
		return false;
	
	FREE_IMAGE_FORMAT fif;
	int flags = 0;
	
	if (_format == "png")
	{
		fif = FIF_PNG;
	}
	else if (_format == "jpg")
	{
		fif = FIF_JPEG;
		flags = quality;
	}
	else if (_format == "bmp")
	{
		fif = FIF_BMP;
	}
	else
	{
		qDebug() << Q_FUNC_INFO << ": unsupported format";
		return false;
	}
	
	return FreeImage_Save(fif, _bitmap, filePath.toLocal8Bit(), flags);
}

bool ImageExporter::pasteImage(const Image &image, const QPoint &pos)
{
	if (!_bitmap)
		return false;
	
	return pasteImageToFIBITMAP(pos, _bitmap, image);
}

bool ImageExporter::pasteSurface(const Surface &surface, const QPoint &pos)
{
	if (!_bitmap)
		return false;
	
	QPointSet keys = surface.keys();
	
	qDebug() << "tile count:" << keys.size();
	
	foreach (const QPoint &key, keys)
	{
		if (pasteImage(surface.tileForKey(key), key * Surface::TileSize + pos) == false)
			return false;
	}
	
	return true;
}


}

