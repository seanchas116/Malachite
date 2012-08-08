#include "mlsurfacepainter.h"
#include "mlsurfaceselection.h"


void MLSurfaceSelection::setPath(const QPainterPath &path)
{
	_type = Path;
	_path = path;
	_surface = MLSurface();
}

void MLSurfaceSelection::setSurface(const MLSurface &surface)
{
	_type = Surface;
	_path = QPainterPath();
	_surface = surface;
}

MLSurface MLSurfaceSelection::clip(const MLSurface &surface) const
{
	if (_type == Whole)
		return surface;
	
	MLSurface result = surface;
	MLSurfacePainter painter(&result);
	painter.setBlendMode(ML::BlendModeDestinationIn);
	
	switch (_type)
	{
	case Path:
		painter.drawPath(_path);
		break;
	case Surface:
		painter.drawSurface(0, 0, _surface);
		break;
	default:
		Q_ASSERT(0);
		break;
	}
	
	painter.flush();
	return result;
}

