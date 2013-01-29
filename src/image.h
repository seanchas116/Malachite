#ifndef MLIMAGE_H
#define MLIMAGE_H

//ExportName: Image

#include "paintable.h"
#include "color.h"
#include "genericimage.h"
#include "pixel.h"

namespace Malachite
{

class MALACHITESHARED_EXPORT Image : public GenericImage<Pixel>, public Paintable
{
public:
	
	typedef GenericImage<Pixel> super;
	
	enum
	{
		BytesPerPixel = 16
	};
	
	/**
	 * Constructs an empty image.
	 */
	Image() : super() {}
	
	/**
	 * Constructs a copy.
	 * @param other The original image
	 */
	Image(const super &other) : super(other) {}
	
	/**
	 * Constructs a image with a size.
	 * The data will not be initialized.
	 * @param size The size
	 */
	Image(const QSize &size) : super(size) {}
	
	/**
	 * Constructs a image with a size.
	 * The data will not be initialized.
	 * @param width The width
	 * @param height The height
	 */
	Image(int width, int height) : super(width, height) {}
	
	/**
	 * Fills the data with 0 (transparent).
	 */
	void clear() { fill(Pixel(0)); }
	
	/**
	 *
	 * @return Whether the image is blank (all pixels are transparent)
	 */
	bool isBlank() const;
	
	/**
	 * Creates a PaintEngine. Used by Painter.
	 * @return a PaintEngine
	 */
	PaintEngine *createPaintEngine();
	
	/**
	 * Fills the background with white and returns the result.
	 * Use when an opaque version of an image is needed (eg JPEG save).
	 * @return The result
	 */
	Image toOpaqueImage() const;
	
	/**
	 * Converts the image into a QImage.
	 * @return The result
	 */
	QImage toQImage() const;
	
	/**
	 * Creates an image from a QImage.
	 * @param qimage The original QImage
	 * @return The result
	 */
	static Image fromQImage(const QImage &qimage);
	
	/**
	 * Converts the image into a QByteArray.
	 * 
	 * @return The result
	 */
	QByteArray toByteArray() const;
	
	/**
	 * Creates an image from a QByteArray.
	 * 
	 * @param data The original QByteArray
	 * @param size The size of the image
	 * @return The result (An empty image if failed)
	 */
	static Image fromByteArray(const QByteArray &data, const QSize &size);
	
	/**
	 * Multiplies each component an value.
	 * @param factor The factor
	 * @return 
	 */
	Image &operator*=(float factor);
};

inline Image operator*(const Image &image, float factor)
{
	Image result = image;
	result *= factor;
	return result;
}

}

Q_DECLARE_METATYPE(Malachite::Image)

#endif // MLIMAGE_H
