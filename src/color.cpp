#include "misc.h"
#include "color.h"

namespace Malachite
{

double Color::component(Component component) const
{
	switch (component)
	{
	case Red:
		return red();
	case Green:
		return green();
	case Blue:
		return blue();
	case Hue:
		return hue();
	case Saturation:
		return saturation();
	case Value:
		return value();
	default:
		return 0;
	}
}

void Color::setComponent(Component component, double x)
{
	switch (component)
	{
	case Red:
		setRed(x);
		break;
	case Green:
		setGreen(x);
		break;
	case Blue:
		setBlue(x);
		break;
	case Hue:
		setHue(x);
		break;
	case Saturation:
		setSaturation(x);
		break;
	case Value:
		setValue(x);
		break;
	default:
		break;
	}
}

void Color::rgbChanged()
{
	double max = max3(_r, _g, _b);
	double min = min3(_r, _g, _b);
	
	_v = max;
	double d = max - min;
	
	if (max) {
		_s = d / max;
	}
	
	if (max == min) {
		return;
	}
	
	double hDegrees;
	
	if (max == _r) {
		hDegrees = 60.0 * (_g - _b) / d;
	} else if (max == _g) {
		hDegrees = 60.0 * (_b - _r) / d + 120.0;
	} else {
		hDegrees = 60.0 * (_r - _g) / d + 240.0;
	}
	
	_h = hDegrees / 360.0;
	_h = _h - floor(_h);
}

void Color::hsvChanged()
{
	double d = _h * 6.0;
	int i = floor(d);
	double f = d - i;
	
	switch (i) {
	case 0:
		_r = _v;
		_g = _v * (1 - (1 - f) * _s);
		_b = _v * (1 - _s);
		break;
	case 1:
		_r = _v * (1 - f * _s);
		_g = _v;
		_b = _v * (1 - _s);
		break;
	case 2:
		_r = _v * (1 - _s);
		_g = _v;
		_b = _v * (1 - (1 - f) * _s);
		break;
	case 3:
		_r = _v * (1 - _s);
		_g = _v * (1 - f * _s);
		_b = _v;
		break;
	case 4:
		_r = _v * (1 - (1 - f) * _s);
		_g = _v * (1 - _s);
		_b = _v;
		break;
	case 5:
		_r = _v;
		_g = _v * (1 - _s);
		_b = _v * (1 - f * _s);
		break;
	default:
		break;
	}
}

QString Color::toWebColor() const
{
	Color color = *this;
	color.setAlpha(1.0);
	BgraU8 pix8 = color.toPixel();
	
	QString rText = QString::number(pix8.r(), 16).toUpper();
	if (rText.size() == 1)
		rText = "0" + rText;
	
	QString gText = QString::number(pix8.g(), 16).toUpper();
	if (gText.size() == 1)
		gText = "0" + gText;
	
	QString bText = QString::number(pix8.b(), 16).toUpper();
	if (bText.size() == 1)
		bText = "0" + bText;
	
	return "#" + rText + gText + bText;
}

Color Color::fromWebColor(const QString &webColor, bool *ok)
{
	if (webColor.size() != 7 || webColor.at(0) != '#')
	{
		if (ok)
			*ok = false;
		return Color();
	}
	
	BgraU8 pix8;
	bool textOk;
	
	pix8.ra() = 0xFF;
	
	pix8.rr() = webColor.mid(1, 2).toInt(&textOk, 16);
	if (!textOk)
	{
		if (ok)
			*ok = false;
		return Color();
	}
	
	pix8.rg() = webColor.mid(3, 2).toInt(&textOk, 16);
	if (!textOk)
	{
		if (ok)
			*ok = false;
		return Color();
	}
	
	pix8.rb() = webColor.mid(5, 2).toInt(&textOk, 16);
	if (!textOk)
	{
		if (ok)
			*ok = false;
		return Color();
	}
	
	if (ok)
		*ok = true;
	
	return Color::fromPixel(pix8);
}

}

