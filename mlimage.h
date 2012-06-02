#ifndef MLIMAGE_H
#define MLIMAGE_H

#include "mlpaintable.h"
#include "mlcolor.h"
#include "mlgenericimage.h"
#include <FreeImage.h>

class MALACHITESHARED_EXPORT MLImage : public MLGenericImage<MLArgb>, public MLPaintable
{
public:
	MLImage() : MLGenericImage<MLArgb>() {}
	MLImage(const MLGenericImage<MLArgb> &other) : MLGenericImage<MLArgb>(other) {}
	MLImage(const QSize &size) : MLGenericImage<MLArgb>(size) {}
	MLImage(int width, int height) : MLGenericImage<MLArgb>(width, height) {}
	
	bool isBlank() const;
	
	MLPaintEngine *createPaintEngine();
	
	static MLImage fromFile(const QString &filePath);
	static MLImage fromFIBITMAP(FIBITMAP *bitmap);
};

Q_DECLARE_METATYPE(MLImage)

#endif // MLIMAGE_H
