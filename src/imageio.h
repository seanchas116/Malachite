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
	
	ImageImporter();
	~ImageImporter();
	
	/**
	 * Loads from a QIODevice.
	 * The device must be random-access.
	 * @param device
	 * @return 
	 */
	bool load(QIODevice *device);
	
	bool load(const QString &filepath);
	
	bool isValid() const;
	
	QSize size() const;
	
	Image toImage() const;
	Surface toSurface(const QPoint &p = QPoint()) const;
	
	static QStringList importableExtensions();
	
private:
	
	class Data;
	Data *d;
};

class MALACHITESHARED_EXPORT ImageExporter
{
public:
	
	ImageExporter(const QString &format);
	~ImageExporter();
	
	void setQuality(int quality);
	
	bool setImage(const Image &image);
	bool setSurface(const Surface &surface, const QRect &rect);
	bool setSurface(const Surface &surface, const QSize &size) { return setSurface(surface, QRect(QPoint(), size)); }
	
	bool pasteImage(const Image &image, const QPoint &pos);
	
	bool save(QIODevice *device);
	bool save(const QString &filePath);
	
private:
	
	class Data;
	Data *d;
};

}


#endif // MLIMAGEIO_H
