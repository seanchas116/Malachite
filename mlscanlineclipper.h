#ifndef MLSCANLINECLIPPER_H
#define MLSCANLINECLIPPER_H

#include <QHash>
#include <cmath>
#include "mlsegmentpath.h"

template<typename T>
class QVectorReverseIterator
{
public:
	QVectorReverseIterator(const QVector<T> &vector) : i(vector) { i.toBack(); }
	
	bool findNext(const T &value) { return i.findPrevious(value); }
	bool findPrevious(const T &value) { return i.findNext(value); }
	bool hasNext() const { return i.hasPrevious(); }
	bool hasPrevious() const { return i.hasNext(); }
	const T &next() { return i.previous(); }
	const T &peekNext() const { return i.peekPrevious(); }
	const T &previous() { return i.next(); }
	const T &peekPrevious() const { return i.peekNext(); }
	void toBack() { i.toFront(); }
	void toFront() { i.toBack(); }
	
	QVectorReverseIterator &operator=(const QVector<T> &vector) { i = vector; i.toBack(); }
	
private:
	QVectorIterator<T> i;
};

typedef QVectorIterator<MLPoint> MLPolygonIterator;
typedef QVectorReverseIterator<MLPoint> MLPolygonReverseIterator;


template <bool transposed>
class MLScanlineClipper
{
public:
	MLScanlineClipper(const MLPolygon &polygon);
	
	int reset();
	QList<QPolygonF> clipNext();
	
	int scanlineMin() const { return _slMin; }
	int scanlineMax() const { return _slMin; }
	int scanline() const { return _sl; }
	int scanlineCount() const { return _slMax - _slMin + 1; }
	
private:
	
	void parsePolygon(const MLPolygon &polygon, QList<MLPolygon> &increasingPolygons, QList<MLPolygon> &decreasingPolygons);
	template <typename PolygonIterator> bool findIntersection(float &column, float row, PolygonIterator &i);
	
	void findIntersections(int row, QHash<int, float> &ic, QHash<int, float> &dc);
	void findNextIntersections();
	
	float elemY(const MLPoint &p) { return transposed ? p.x() : p.y(); }
	float elemX(const MLPoint &p) { return transposed ? p.y() : p.x(); }
	
	int _slMax, _slMin, _sl;
	
	QHash<int, float> _lic, _ic, _ldc, _dc; // current and last scanline intersection point
	
	QList<MLPolygon> _ip;	// polygons which are row-increasing
	QList<MLPolygon> _dp;	// polygons which are row-decreasing
	
	QList<MLPolygonIterator> _ii, _lii; // current iterator and last iterator of ip
	QList<MLPolygonReverseIterator> _di, _ldi; // current iterator and last iterator of dp
	
	int _sc;	// ip, dp's count
};

template <bool transposed>
MLScanlineClipper<transposed>::MLScanlineClipper(const MLPolygon &polygon)
{
	// parse and split polygons
	parsePolygon(polygon, _ip, _dp);
	_sc = _ip.size();
	
	// if polygon is split -decreasing- -increasing- -decreasing- -increasing- ... order
	// adjust polygon split to -increasing- -decreasing- ... order
	if (_ip.at(0).at(0) != polygon.at(0))
	{
		QPolygonF p = _dp.at(0);
		
		for (int i = 1; i < _ip.size(); ++i)
		{
			_dp[i-1] = _dp.at(i);
		}
		
		_dp[_dp.size() - 1] = p;
	}
	
	// find y max/min
	
	QList<float> slMaxCandidate, slMinCandidate;
	slMaxCandidate.reserve(sectionCount);
	slMinCandidate.reserve(sectionCount);
	
	for (int i = 0; i < _sc; ++i)
	{
		slMaxCandidate << elemColumn(_ip.at(i).last());
		slMinCandidate << elemColumn(_ip.at(i).first());
	}
	
	float slMaxF = *std::max_element(slMaxCandidate.begin(), slMaxCandidate.end());
	float slMinF = *std::min_element(slMinCandidate.begin(), slMinCandidate.end());
	
	_slMax = ceilf(slMaxF);
	_slMin = floorf(slMinF);
	
	reset();
}

template <bool transposed>
void MLScanlineClipper<transposed>::reset()
{
	// intializing scanline index
	
	_sl = _slMin - 1;
	
	// initializing intersections
	
	findIntersections(_sl+1, _ic, _dc);
	
	// initializing iterators
	
	_ii.clear();
	_di.clear();
	
	_ii.reserve(sectionCount);
	_di.reserve(sectionCount);
	
	for (int i = 0; i < _sc; ++i)
	{
		_ii << QPolygonFIterator(_ip.at(i));
		_di << QPolygonFReverseIterator(_dp.at(i));
	}
}

template <bool transposed>
void MLScanlineClipper<transposed>::findNextIntersections()
{
	_sl++;
	_lic = _ic;
	_ldc = _dc;
	_lii = _ii;
	_ldi = _di;
	findIntersections(_sl+1, _ic, _dc);
}

template <bool transposed>
QList<QPolygonF> MLScanlineClipper<transposed>::clipNext()
{
	findNextIntersections();
	
	
}

// Split the polygon into segments. In each segments, all y values of the points will be increasing or decreasing.
template <bool transposed>
void MLScanlineClipper<transposed>::parsePolygon(const MLPolygon &originalPolygon, QList<MLPolygon> &increasingPolygons, QList<MLPolygon> &decreasingPolygons)
{
	MLPolygon polygon = originalPolygon;
	if (polygon.first() != polygon.last())
		polygon << polygon.first();
	
	bool isIncreasing = elemY(polygon.at(1)) >= elemY(polygon.at(0));
	bool startIncreasing = isIncreasing;
	int prevIndex = 0;
	int count = polygon.size() - 1;
	int i;
	
	for (i = 1; i < count; ++i)
	{
		if (elemY(polygon.at(i+1)) >= elemY(polygon.at(i)))
		{
			if (!isIncreasing)
			{
				isIncreasing = true;
				decreasingPolygons << polygon.mid(prevIndex, i - prevIndex + 1);
				prevIndex = i;
			}
		}
		else
		{
			if (isIncreasing)
			{
				isIncreasing = false;
				increasingPolygons << polygon.mid(prevIndex, i - prevIndex + 1);
				prevIndex = i;
			}
		}
	}
	
	if (isIncreasing)
	{
		if (startIncreasing)
			increasingPolygons[0] = polygon.mid(prevIndex, i - prevIndex) + increasingPolygons.at(0);
		else
			increasingPolygons << polygon.mid(prevIndex, i - prevIndex + 1);
	}
	else
	{
		if (!startIncreasing)
			decreasingPolygons[0] = polygon.mid(prevIndex, i - prevIndex) + decreasingPolygons.at(0);
		else
			decreasingPolygons << polygon.mid(prevIndex, i - prevIndex + 1);
	}
	
	Q_ASSERT(increasingPolygons.size() == decreasingPolygons.size());
}

template <bool transposed, typename PolygonIterator>
bool MLScanlineClipper<transposed>::findIntersection(float &x, float y, PolygonIterator &i)
{
	PolygonIterator back = i;
	back.toBack();
	
	if (elemY(back.peekPrevious()) < y)
	{
		i = back;
		return false;
	}
	
	MLPoint p1 = i.peekNext();
	
	if (elemY(p1) > y)
	{
		return false;
	}
	if (elemY(p1) == y)
	{
		x = elemX(p1);
		return true;
	}
	
	i.next();
	
	for (; i.hasNext(); i.next())
	{
		p1 = i.peekNext();
		
		if (elemY(p1) > y)
		{
			MLPoint p0 = i.peekPrevious();
			x = ((y - elemY(p0.y())) * elemX(p1) + (elemY(p1) - y) * elemX(p0)) / (elemY(p1) - elemY(p0));
			i.previous();
			return true;
		}
		if (elemY(p1) == y)
		{
			x = elemX(p1);
			return true;
		}
	}
	
	Q_ASSERT(0);
	return false;
}

template <bool transposed>
void MLScanlineClipper<transposed>::findIntersections(int y, QHash<int, float> &ic, QHash<int, float> &dc)
{
	float x;
	
	for (int i = 0; i < _sc; ++i)
	{
		if (findIntersection(x, y, _ii.at(i)))
			ic[i] = column;
		if (findIntersection(x, y, _di.at(i)))
			dc[i] = column;
	}
	
	return intersections;
}

#endif // MLSCANLINECLIPPER_H
