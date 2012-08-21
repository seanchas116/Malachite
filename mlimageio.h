#ifndef MLIMAGEIO_H
#define MLIMAGEIO_H

#include <QDebug>
#include "mlsurface.h"

struct FIBITMAP;

class MALACHITESHARED_EXPORT MLImageImporter
{
public:
	
	MLImageImporter() : _bitmap(0) {}
	MLImageImporter(const QString &filePath);
	~MLImageImporter();
	
	bool isValid() const { return _bitmap; }
	
	FIBITMAP *bitmap() { return _bitmap; }
	
	QSize size() const { return _size; }
	
	MLImage toImage() const;
	MLSurface toSurface(const QPoint &p = QPoint()) const;
	
private:
	
	FIBITMAP *_bitmap;
	QSize _size;
};

class MALACHITESHARED_EXPORT MLImageExporter
{
public:
	
	MLImageExporter(const QSize &size, const QString &format);
	//MLImageExporter(const MLSurface &surface, const QSize &size, const QString &format);
	MLImageExporter(const MLImage &image, const QString &format);
	
	~MLImageExporter();
	
	bool save(const QString &filePath, int quality = 80);
	
private:
	
	bool pasteImage(const MLImage &image, const QPoint &pos);
	//bool pasteSurface(const MLSurface &surface, const QPoint &pos);
	
	QSize _size;
	FIBITMAP *_bitmap;
	QString _format;
};


#endif // MLIMAGEIO_H
