#ifndef MLIMAGE_H
#define MLIMAGE_H

#include "mlpaintable.h"
#include "mlcolor.h"
#include "mlgenericsharedimage.h"

class MALACHITESHARED_EXPORT MLImage : public MLGenericSharedImage<MLArgb>, public MLPaintable
{
public:
	MLImage() : MLGenericSharedImage<MLArgb>() {}
	MLImage(const MLGenericSharedImage<MLArgb> &other) : MLGenericSharedImage<MLArgb>(other) {}
	MLImage(const QSize &size) : MLGenericSharedImage<MLArgb>(size) {}
	MLImage(int width, int height) : MLGenericSharedImage<MLArgb>(width, height) {}
	
	bool isBlank() const;
	
	MLPaintEngine *createPaintEngine();
	
	QImage toQImage() const;
	
	static MLImage fromQImage(const QImage &qimage);
};

Q_DECLARE_METATYPE(MLImage)



#endif // MLIMAGE_H
