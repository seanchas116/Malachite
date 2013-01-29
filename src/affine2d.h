#pragma once

//ExportName: Affine2D

#include <QTransform>
#include <cmath>
#include "mat2d.h"

namespace Malachite
{

class Affine2D
{
public:
	
	Affine2D() {}
	Affine2D(const Mat2D &mat2, const Vec2D &delta) : _m(mat2), _v(delta) {}
	Affine2D(const Affine2D &other) : _m(other._m), _v(other._v) {}
	Affine2D(double a11, double a12, double a21, double a22, double dx, double dy) :
	    _m(a11, a12, a21, a22), _v(dx, dy) {}
	
	static Affine2D fromTranslation(const Vec2D &delta)
	{
		return Affine2D(Mat2D(), delta);
	}
	
	static Affine2D fromScale(double sx, double sy)
	{
		return Affine2D(Mat2D(sx, 0, 0, sy), Vec2D());
	}
	
	static Affine2D fromScale(double s)
	{
		return fromScale(s, s);
	}
	
	static Affine2D fromScale(const Vec2D &v)
	{
		return fromScale(v.x(), v.y());
	}
	
	static Affine2D fromRotation(double rotation)
	{
		double c = std::cos(rotation);
		double s = std::sin(rotation);
		return Affine2D(Mat2D(c, -s, s, c), Vec2D());
	}
	
	double a11() const { return _m.a11(); }
	double a12() const { return _m.a12(); }
	double a21() const { return _m.a21(); }
	double a22() const { return _m.a22(); }
	double dx() const { return _v.x(); }
	double dy() const { return _v.y(); }
	
	double &ra11() { return _m.ra11(); }
	double &ra12() { return _m.ra12(); }
	double &ra21() { return _m.ra21(); }
	double &ra22() { return _m.ra22(); }
	double &rdx() { return _v.rx(); }
	double &rdy() { return _v.ry(); }
	
	Mat2D mat() const { return _m; }
	Vec2D delta() const { return _v; }
	
	Mat2D &rmat() { return _m; }
	Vec2D &rdelta() { return _v; }
	
	double determinant() const { return _m.determinant(); }
	
	Affine2D inverted() const
	{
		double det = determinant();
		
		if (!det)
			return Affine2D();
		
		double detInv = 1.0 / det;
		
		Mat2D m = detInv * _m.transposedCofactorMat();
		
		Mat2D m1(_m.v1(), _v);
		Mat2D m2(_m.v2(), _v);
		
		Vec2D v = detInv * Vec2D(m2.determinant(), -m1.determinant());
		
		return Affine2D(m, v);
	}
	
	QTransform toQTransform() const
	{
		return QTransform(a11(), a21(), a12(), a22(), dx(), dy());
	}
	
private:
	
	Mat2D _m;
	Vec2D _v;
};

inline Affine2D operator*(const Affine2D &m1, const Affine2D &m2)
{
	return Affine2D(m1.mat() * m2.mat(), m1.mat() * m2.delta() + m1.delta());
}

inline Vec2D operator*(const Affine2D &m, const Vec2D &v)
{
	return m.mat() * v + m.delta();
}

}

