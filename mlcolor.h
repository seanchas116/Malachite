#ifndef MLCOLOR_H
#define MLCOLOR_H

#include <stdint.h>
#include <cstdlib>
#include <cmath>
#include <QColor>

#include "mlglobal.h"
#include "mlmisc.h"
#include "mlssevector.h"

// Color store is inverted (Little Endian)

struct MLArgb8;
struct MLRgb8;
struct MLRgba8;
struct MLRgb16;
struct MLRgba16;
struct MLArgb32;
struct MLFastArgb8;
struct MLFastArgb32;
struct MLArgbF;
struct MLFastArgbF;

struct MALACHITESHARED_EXPORT MLArgb8
{
	union {
		struct {
			uint8_t b;
			uint8_t g;
			uint8_t r;
			uint8_t a;
		};
		uint32_t data;
	};
	
	MLArgb8 &operator=(const MLArgbF &src);
	MLArgb8 &operator=(const MLFastArgbF &src);
};

struct MALACHITESHARED_EXPORT MLRgb8
{
	uint8_t b;
	uint8_t g;
	uint8_t r;
	
	MLRgb8 &operator=(const MLFastArgbF &src);
};

struct MALACHITESHARED_EXPORT MLRgba8
{
	union {
		struct {
			uint8_t a;
			uint8_t b;
			uint8_t g;
			uint8_t r;
		};
		uint32_t data;
	};
	
	MLRgba8 &operator=(const MLArgbF &src);
	MLRgba8 &operator=(const MLFastArgbF &src);
};

struct MALACHITESHARED_EXPORT MLRgb16
{
	uint16_t b;
	uint16_t g;
	uint16_t r;
	
	MLRgb16 &operator=(const MLFastArgbF &src);
};

struct MALACHITESHARED_EXPORT MLRgba16
{
	union {
		struct {
			uint16_t a;
			uint16_t b;
			uint16_t g;
			uint16_t r;
		};
		uint64_t data;
	};
	
	MLRgba16 &operator=(const MLArgbF &src);
	MLRgba16 &operator=(const MLFastArgbF &src);
};

struct MALACHITESHARED_EXPORT MLArgb32
{
	union {
		struct {
			uint32_t b;
			uint32_t g;
			uint32_t r;
			uint32_t a;
		};
		__v4si v;
	};
	
	ML_ALIGN_16BYTE
};

struct MALACHITESHARED_EXPORT MLFastArgb8
{
	union {
		struct {
			uint8_t b;
			uint8_t g;
			uint8_t r;
			uint8_t a;
		};
		uint32_t data;
	};
	
	MLFastArgb8 &operator=(const MLFastArgbF &src);
};

struct MALACHITESHARED_EXPORT MLFastArgb32
{
	union {
		struct {
			uint32_t b;
			uint32_t g;
			uint32_t r;
			uint32_t a;
		};
		__v4si v;
	};
	
	ML_ALIGN_16BYTE
};

struct MALACHITESHARED_EXPORT MLArgbF
{
	union {
		struct {
			float b;
			float g;
			float r;
			float a;
		};
		__v4sf v;
	};
	
	MLArgbF &operator=(const MLRgb8 &src);
	MLArgbF &operator=(const MLRgb16 &src);
	MLArgbF &operator=(const MLRgba8 &src);
	MLArgbF &operator=(const MLRgba16 &src);
	MLArgbF &operator=(const MLFastArgbF &src);
	
	ML_ALIGN_16BYTE
};

struct MALACHITESHARED_EXPORT MLFastArgbF
{
	union {
		struct {
			float b;
			float g;
			float r;
			float a;
		};
		__v4sf v;
	};
	
	MLFastArgbF &operator=(const MLRgb8 &src);
	MLFastArgbF &operator=(const MLRgb16 &src);
	MLFastArgbF &operator=(const MLFastArgb8 &src);
	MLFastArgbF &operator=(const MLRgba8 &src);
	MLFastArgbF &operator=(const MLRgba16 &src);
	MLFastArgbF &operator=(const MLArgbF &src);
	
	ML_ALIGN_16BYTE
};

inline MLArgb8 &MLArgb8::operator=(const MLArgbF &src)
{
	MLArgb32 argb32;
	argb32.v = __builtin_ia32_cvtps2dq(src.v * mlFloatToVector(0xFF));
	a = argb32.a;
	r = argb32.r;
	g = argb32.g;
	b = argb32.b;
	return *this;
}

inline MLArgb8 &MLArgb8::operator=(const MLFastArgbF &src)
{
	MLArgbF argbf;
	argbf = src;
	return *this = argbf;
}

inline MLRgb8 &MLRgb8::operator=(const MLFastArgbF &src)
{
	MLFastArgb32 argb32;
	MLFastArgbF argb;
	argb.v = src.v + mlFloatToVector(1.0f - src.a);
	
	argb32.v = __builtin_ia32_cvtps2dq(argb.v * mlFloatToVector(0xFF));
	r = argb32.r;
	g = argb32.g;
	b = argb32.b;
	
	return *this;
}

inline MLRgba8 &MLRgba8::operator=(const MLArgbF &src)
{
	MLArgb32 argb32;
	argb32.v = __builtin_ia32_cvtps2dq(src.v * mlFloatToVector(0xFF));
	a = argb32.a;
	r = argb32.r;
	g = argb32.g;
	b = argb32.b;
	return *this;
}

inline MLRgba8 &MLRgba8::operator=(const MLFastArgbF &src)
{
	MLArgbF argbf;
	argbf = src;
	return *this = argbf;
}

inline MLRgb16 &MLRgb16::operator=(const MLFastArgbF &src)
{
	MLFastArgb32 argb32;
	MLFastArgbF argb;
	argb.v = src.v + mlFloatToVector(1.0f - src.a);
	
	argb32.v = __builtin_ia32_cvtps2dq(argb.v * mlFloatToVector(0xFFFF));
	r = argb32.r;
	g = argb32.g;
	b = argb32.b;
	
	return *this;
}

inline MLRgba16 &MLRgba16::operator=(const MLArgbF &src)
{
	MLArgb32 argb32;
	argb32.v = __builtin_ia32_cvtps2dq(src.v * mlFloatToVector(0xFFFF));
	a = argb32.a;
	r = argb32.r;
	g = argb32.g;
	b = argb32.b;
	return *this;
}

inline MLRgba16 &MLRgba16::operator=(const MLFastArgbF &src)
{
	MLArgbF argbf;
	argbf = src;
	return *this = argbf;
}

inline MLFastArgb8 &MLFastArgb8::operator=(const MLFastArgbF &src)
{
	MLFastArgb32 argb32;
	argb32.v = __builtin_ia32_cvtps2dq(src.v * mlFloatToVector(0xFF));
	a = argb32.a;
	r = argb32.r;
	g = argb32.g;
	b = argb32.b;
	return *this;
}

inline MLArgbF &MLArgbF::operator=(const MLRgb8 &src)
{
	MLArgb32 argb32;
	argb32.a = 0xFF;
	argb32.r = src.r;
	argb32.g = src.g;
	argb32.b = src.b;
	v = __builtin_ia32_cvtdq2ps(argb32.v) / mlFloatToVector(0xFF);
	return *this;
}

inline MLArgbF &MLArgbF::operator=(const MLRgb16 &src)
{
	MLArgb32 argb32;
	argb32.a = 0xFFFF;
	argb32.r = src.r;
	argb32.g = src.g;
	argb32.b = src.b;
	v = __builtin_ia32_cvtdq2ps(argb32.v) / mlFloatToVector(0xFFFF);
	return *this;
}

inline MLArgbF &MLArgbF::operator=(const MLRgba8 &src)
{
	MLArgb32 argb32;
	argb32.a = src.a;
	argb32.r = src.r;
	argb32.g = src.g;
	argb32.b = src.b;
	v = __builtin_ia32_cvtdq2ps(argb32.v) / mlFloatToVector(0xFF);
	return *this;
}

inline MLArgbF &MLArgbF::operator=(const MLRgba16 &src)
{
	MLArgb32 argb32;
	argb32.a = src.a;
	argb32.r = src.r;
	argb32.g = src.g;
	argb32.b = src.b;
	v = __builtin_ia32_cvtdq2ps(argb32.v) / mlFloatToVector(0xFFFF);
	return *this;
}

inline MLArgbF &MLArgbF::operator=(const MLFastArgbF &src)
{
	MLArgbF divisor;
	divisor.a = 1.0f;
	divisor.r = src.a;
	divisor.g = src.a;
	divisor.b = src.a;
	v = src.v / divisor.v;
	return *this;
}

inline MLFastArgbF &MLFastArgbF::operator=(const MLRgb8 &src)
{
	MLFastArgb32 argb32;
	argb32.a = 0xFF;
	argb32.r = src.r;
	argb32.g = src.g;
	argb32.b = src.b;
	v = __builtin_ia32_cvtdq2ps(argb32.v) / mlFloatToVector(0xFF);
	return *this;
}

inline MLFastArgbF &MLFastArgbF::operator=(const MLRgb16 &src)
{
	MLFastArgb32 argb32;
	argb32.a = 0xFFFF;
	argb32.r = src.r;
	argb32.g = src.g;
	argb32.b = src.b;
	v = __builtin_ia32_cvtdq2ps(argb32.v) / mlFloatToVector(0xFFFF);
	return *this;
}

inline MLFastArgbF &MLFastArgbF::operator=(const MLFastArgb8 &src)
{
	MLFastArgb32 argb32;
	argb32.a = src.a;
	argb32.r = src.r;
	argb32.g = src.g;
	argb32.b = src.b;
	v = __builtin_ia32_cvtdq2ps(argb32.v) / mlFloatToVector(0xFF);
	return *this;
}

inline MLFastArgbF &MLFastArgbF::operator=(const MLRgba8 &src)
{
	MLArgbF argbf;
	argbf = src;
	return *this = argbf;
}

inline MLFastArgbF &MLFastArgbF::operator=(const MLRgba16 &src)
{
	MLArgbF argbf;
	argbf = src;
	return *this = argbf;
}

inline MLFastArgbF &MLFastArgbF::operator=(const MLArgbF &src)
{
	MLArgbF factor;
	factor.a = 1.0f;
	factor.r = src.a;
	factor.g = src.a;
	factor.b = src.a;
	v = src.v * factor.v;
	return *this;
}

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
	
	MLFastArgbF toFastArgbF() const
	{
		MLFastArgbF argb;
		argb.a = _a;
		argb.r = _r * _a;
		argb.g = _g * _a;
		argb.b = _b * _a;
		return argb;
	}
	
	MLFastArgb8 toFastArgb8() const
	{
		MLFastArgb8 argb8;
		argb8 = toFastArgbF();
		return argb8;
	}
	
	QColor toQColor() const
	{
		return QColor::fromRgbF(_r, _g, _b, _a);
	}
	
	QString toWebColor() const;
	
	static MLColor fromFastArgbF(const MLFastArgbF &argb)
	{
		return MLColor::fromRgb(argb.r / argb.a, argb.g / argb.a, argb.b / argb.a, argb.a);
	}
	
	static MLColor fromFastArgb8(const MLFastArgb8 &argb)
	{
		MLFastArgbF argbf;
		argbf = argb;
		return MLColor::fromFastArgbF(argbf);
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
Q_DECLARE_METATYPE(MLFastArgbF)

#endif // MLCOLOR_H
