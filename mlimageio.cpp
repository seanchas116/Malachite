#include <QtGui>
#include "mlimageio.h"
#include "private/mlimageioprivate.h"

#include <FreeImage.h>

MLImageImporter::MLImageImporter(const QString &filePath) :
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

MLImageImporter::~MLImageImporter()
{
	if (_bitmap) FreeImage_Unload(_bitmap);
}

MLImage MLImageImporter::toImage() const
{
	if (!_bitmap)
		return MLImage();
	
	MLImage image(size());
	
	mlPasteFIBITMAPToImage(QPoint(), &image, _bitmap);
	return image;
}

MLSurface MLImageImporter::toSurface(const QPoint &p) const
{
	if (!_bitmap)
		return MLSurface();
	
	MLSurface surface;
	mlPasteFIBITMAPToImage(p, &surface, _bitmap);
	return surface;
}

MLImageExporter::MLImageExporter(const QSize &size, const QString &format) :
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

/*
MLImageExporter::MLImageExporter(const MLSurface &surface, const QSize &size, const QString &format) :
	MLImageExporter(size, format)
{
	pasteSurface(surface, QPoint());
}*/

MLImageExporter::MLImageExporter(const MLImage &image, const QString &format) :
	MLImageExporter(image.size(), format)
{
	pasteImage(image, QPoint());
}

MLImageExporter::~MLImageExporter()
{
	if (_bitmap)
		FreeImage_Unload(_bitmap);
}

bool MLImageExporter::save(const QString &filePath, int quality)
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
	
	return FreeImage_Save(fif, _bitmap, filePath.toLocal8Bit(), flags);
}

bool MLImageExporter::pasteImage(const MLImage &image, const QPoint &pos)
{
	if (!_bitmap)
		return false;
	
	return mlPasteImageToBitmap(pos, _bitmap, image);
}

/*
bool MLImageExporter::pasteSurface(const MLSurface &surface, const QPoint &pos)
{
	if (!_bitmap)
		return false;
	
	QPointSet keys = surface.keys();
	
	qDebug() << "tile count:" << keys.size();
	
	foreach (const QPoint &key, keys)
	{
		if (pasteImage(surface.tileForKey(key), key * MLSurface::TileSize + pos) == false)
			return false;
	}
	
	return true;
}*/

/*
bool mlSaveAsPNG(const MLImage &image, const QString &filePath)
{
	FIBITMAP *bitmap = FreeImage_AllocateT(FIT_RGBA16, image.width(), image.height());
	
	auto wrapper = MLGenericImage<ML::ImageFormatArgb, MLVec4U16>::wrap(FreeImage_GetBits(bitmap), image.size(), FreeImage_GetPitch(bitmap));
	wrapper.paste(image, true);
	
	return FreeImage_Save(FIF_PNG, bitmap, filePath.toLocal8Bit());
}

bool mlSaveAsBMP(const MLImage &image, const QString &filePath)
{
	MLImage opaqueImage = image.toOpaqueImage();
	
	FIBITMAP *bitmap = FreeImage_Allocate(image.width(), image.height(), 24);
	
	auto wrapper = MLGenericImage<ML::ImageFormatRgb, MLVec3U8>::wrap(FreeImage_GetBits(bitmap), image.size(), FreeImage_GetPitch(bitmap));
	wrapper.paste(opaqueImage, true);
	
	return FreeImage_Save(FIF_BMP, bitmap, filePath.toLocal8Bit());
}

bool mlSaveAsJPEG(const MLImage &image, const QString &filePath)
{
	MLImage opaqueImage = image.toOpaqueImage();
	
	FIBITMAP *bitmap = FreeImage_Allocate(image.width(), image.height(), 24);
	
	auto wrapper = MLGenericImage<ML::ImageFormatRgb, MLVec3U8>::wrap(FreeImage_GetBits(bitmap), image.size(), FreeImage_GetPitch(bitmap));
	wrapper.paste(opaqueImage, true);
	
	return FreeImage_Save(FIF_JPEG, bitmap, filePath.toLocal8Bit());
}
*/

