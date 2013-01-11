#ifndef MLIMAGEIO_H
#define MLIMAGEIO_H

//ExportName: ImageIO

#include <QDebug>
#include "surface.h"

struct FIBITMAP;

namespace Malachite
{

class MALACHITESHARED_EXPORT ImageImporter
{
public:
	
	ImageImporter() : _bitmap(0) {}
	ImageImporter(const QString &filePath);
	~ImageImporter();
	
	bool isValid() const { return _bitmap; }
	
	FIBITMAP *bitmap() { return _bitmap; }
	
	QSize size() const { return _size; }
	
	Image toImage() const;
	Surface toSurface(const QPoint &p = QPoint()) const;
	
	static QStringList importableExtensions();
	
private:
	
	FIBITMAP *_bitmap;
	QSize _size;
};

class MALACHITESHARED_EXPORT ImageExporter
{
public:
	
	ImageExporter(const QSize &size, const QString &format);
	ImageExporter(const Surface &surface, const QSize &size, const QString &format);
	ImageExporter(const Image &image, const QString &format);
	
	~ImageExporter();
	
	bool setImage(const Image &image, const QPoint &pos);
	bool setSurface(const Surface &surface, const QPoint &pos);
	
	bool save(const QString &filePath, int quality = 80);
	
private:
	
	QSize _size;
	FIBITMAP *_bitmap;
	QString _format;
};

}


#endif // MLIMAGEIO_H
