#include <QtGui>

#include "viewport.h"

using namespace Malachite;

Viewport::Viewport(QWidget *parent) :
    QWidget(parent),
    _image(800, 600)
{
}

void Viewport::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	
	switch (_mode)
	{
		default:
		case ModeImage:
		{
			painter.drawImage(QPoint(),  _image.toQImage());
			break;
		}
		case ModeSurface:
		{
			for (const QPoint &key : _surface.keys())
				painter.drawImage(key * Surface::tileWidth(), _surface.tile(key).toQImage());
			break;
		}
	}
}
