#ifndef MLSURFACEPAINTENGINE_H
#define MLSURFACEPAINTENGINE_H

#include <QPaintEngine>
#include "mlsurface.h"
#include "mlpaintengine.h"

class MLSurfacePaintEngine : public MLPaintEngine
{
public:
	
	MLSurfacePaintEngine();
	~MLSurfacePaintEngine();
	
	bool begin(MLPaintable *paintable);
	bool flush();
	
	void drawEllipse(double x, double y, double rx, double ry);
	void drawPath(const QPainterPath &path);
	void drawImage(const QPoint &point, const MLImage &image);
	
	void setKeyClip(const QPointSet &keys) { _keyClip = keys; }
	QPointSet keyClip() const { return _keyClip; }
	
	void deleteTile(const QPoint &key);
	void clear();
	MLImage *replaceTile(const QPoint &key, MLImage *image);
	MLImage *takeTile(const QPoint &key);
	
	void replace(const MLSurface &surface, const QPointSet &keys);
	
	void updateState(const MLPaintEngineState &state);
	
	QPointSet editedKeys() const { return _editedKeys; }
	
protected:
	
	MLImage *tileRefForKey(const QPoint &key);
	MLImage *tileRefForKey(int x, int y) { return tileRefForKey(QPoint(x, y)); }
	const MLImage *constTileRefForKey(const QPoint &key) const;
	const MLImage *constTileRefForKey(int x, int y) const { return constTileRefForKey(QPoint(x, y)); }
	const MLImage *tileRefForKey(const QPoint &key) const { return constTileRefForKey(key); }
	const MLImage *tileRefForKey(int x, int y) const { return constTileRefForKey(QPoint(x, y)); }
	
private:
	
	MLPaintEngineState _state;
	MLSurface *_surface;
	QPointSet _editedKeys, _keyClip;
};

#endif // MLSURFACEPAINTENGINE_H
