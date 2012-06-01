#ifndef MLCOLOR_H
#define MLCOLOR_H

#include <cstdlib>
#include <cmath>
#include <QColor>

#include "mlmisc.h"
#include "mlargb.h"

class MALACHITESHARED_EXPORT MLColor
{
public:
	
	enum Component
	{
		Red,
		Green,
		Blue,
		Hue,
		Saturation,
		Value
	};
	
	MLColor() :
		_a(0), _r(0), _g(0), _b(0), _h(0), _s(0), _v(0) {}
	
	double alpha() const { return _a; }
	double red() const { return _r; }
	double green() const { return _g; }
	double blue() const { return _b; }
	double hue() const { return _h; }
	double saturation() const { return _s; }
	double value() const { return _v; }
	double component(Component component) const;
	double intervaledComponent(Component component) const;	// return value is in [0, 1]
	
	void setAlpha(double a)
	{
		_a = qBound(0.0, a, 1.0);
	}
	
	void setRed(double r)
	{
		_r = qBound(0.0, r, 1.0);
		rgbChanged();
	}
	
	void setGreen(double g)
	{
		_g = qBound(0.0, g, 1.0);
		rgbChanged();
	}
	
	void setBlue(double b)
	{
		_b = qBound(0.0, b, 1.0);
		rgbChanged();
	}
	
	void setHue(double h)
	{
		_h = fmod(h, 360.0);
		if (_h < 0)
			_h += 360.0;
		hsvChanged();
	}
	
	void setSaturation(double s)
	{
		_s = qBound(0.0, s, 1.0);
		hsvChanged();
	}
	
	void setValue(double v)
	{
		_v = qBound(0.0, v, 1.0);
		hsvChanged();
	}
	
	void setRgb(double r, double g, double b)
	{
		_r = qBound(0.0, r, 1.0);
		_g = qBound(0.0, g, 1.0);
		_b = qBound(0.0, b, 1.0);
		rgbChanged();
	}
	
	void setHsv(double h, double s, double v)
	{
		_h = fmod(h, 360.0);
		if (_h < 0)
			_h += 360.0;
		_s = qBound(0.0, s, 1.0);
		_v = qBound(0.0, v, 1.0);
		hsvChanged();
	}
	
	void setComponent(Component component, double x);
	void setIntervaledComponent(Component component, double x);
	
	static MLColor fromRgb(double r, double g, double b, double a = 1.0)
	{
		MLColor color;
		color.setAlpha(a);
		color.setRgb(r, g, b);
		color.rgbChanged();
		return color;
	}

	static MLColor fromHsv(double h, double s, double v, double a = 1.0)
	{
		MLColor color;
		color.setAlpha(a);
		color.setHsv(h, s, v);
		color.hsvChanged();
		return color;
	}
	
	MLArgb toArgb() const
	{
		MLArgb argb;
		argb.a() = _a;
		argb.r() = _r * _a;
		argb.g() = _g * _a;
		argb.b() = _b * _a;
		return argb;
	}
	
	MLFastArgb8 toFastArgb8() const
	{
		MLFastArgb8 argb8;
		argb8 = toArgb();
		return argb8;
	}
	
	QColor toQColor() const
	{
		return QColor::fromRgbF(_r, _g, _b, _a);
	}
	
	QString toWebColor() const;
	
	static MLColor fromArgb(const MLArgb &argb)
	{
		return argb.a() == 0 ? MLColor::fromRgb(argb.r() / argb.a(), argb.g() / argb.a(), argb.b() / argb.a(), argb.a()) : MLColor();
	}
	
	static MLColor fromFastArgb8(const MLFastArgb8 &argb)
	{
		MLArgb argbf;
		argbf = argb;
		return MLColor::fromArgb(argbf);
	}
	
	static MLColor fromQColor(const QColor &qcolor)
	{
		return MLColor::fromRgb(qcolor.redF(), qcolor.greenF(), qcolor.blueF(), qcolor.alphaF());
	}
	
	static MLColor fromWebColor(const QString &webColor, bool *ok = 0);
	
	bool operator==(const MLColor &other)
	{
		return	_a == other._a && 
				_r == other._r &&
				_g == other._g &&
				_b == other._b &&
				_h == other._h &&
				_s == other._s &&
				_v == other._v;
	}
	
private:
	
	void rgbChanged();
	void hsvChanged();
	
	double _a, _r, _g, _b, _h, _s, _v;
};

Q_DECLARE_METATYPE(MLColor)

#endif // MLCOLOR_H
