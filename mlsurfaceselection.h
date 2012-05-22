#ifndef MLSURFACESELECTION_H
#define MLSURFACESELECTION_H

#include "mlsurface.h"
#include <QPainterPath>

class MALACHITESHARED_EXPORT MLSurfaceSelection
{
public:
	
	enum Type
	{
		Path,
		Surface,
		Whole
	};
	
	MLSurfaceSelection() : _type(Whole) {}
	MLSurfaceSelection(const QPainterPath &path) : _type(Path), _path(path) {}
	MLSurfaceSelection(const MLSurface &surface) : _type(Surface), _surface(surface) {}
	
	Type type() const { return _type; }
	
	void setPath(const QPainterPath &path);
	QPainterPath path() const { return _path; }
	
	void setSurface(const MLSurface &surface);
	MLSurface surface() const { return _surface; }
	
	MLSurface clip(const MLSurface &surface) const;
	
private:
	
	Type _type;
	QPainterPath _path;
	MLSurface _surface;
};

#endif // MLSURFACESELECTION_H
