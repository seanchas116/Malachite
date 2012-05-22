#ifndef MLIMAGE_H
#define MLIMAGE_H

#include "mlpaintable.h"
#include "mlcolor.h"
#include "mlgenericimage.h"
//#include <FreeImage.h>

class MALACHITESHARED_EXPORT MLImage : public MLGenericImage<MLFastArgbF>, public MLPaintable
{
public:
	MLImage() : MLGenericImage<MLFastArgbF>() {}
	MLImage(const MLGenericImage<MLFastArgbF> &other) : MLGenericImage<MLFastArgbF>(other) {}
	MLImage(const QSize &size) : MLGenericImage<MLFastArgbF>(size) {}
	MLImage(int width, int height) : MLGenericImage<MLFastArgbF>(width, height) {}
	
	MLPaintEngine *createPaintEngine();
	
	//static MLImage fromFile(const QString &filePath);
	//static MLImage fromFIBITMAP(FIBITMAP *bitmap);
};

Q_DECLARE_METATYPE(MLImage)

#endif // MLIMAGE_H
