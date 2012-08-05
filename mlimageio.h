#ifndef MLIMAGEIO_H
#define MLIMAGEIO_H

#include <QDebug>
#include <QObject>
#include "mlsurface.h"
#include "mlgenericwrapperimage.h"

struct FIBITMAP;

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
	
	QSize size() const { return _size; }
	
	MLImage toImage() const;
	MLSurface toSurface(const QPoint &p = QPoint()) const;
	
private:
	FIBITMAP *_bitmap;
	QSize _size;
};

#endif // MLIMAGEIO_H
