#ifndef MLSCANLINECLIPPER_H
#define MLSCANLINECLIPPER_H

#include <QVector>
#include <QPolygonF>

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

typedef QVectorIterator<QPointF> QPolygonFIterator;
typedef QVectorReverseIterator<QPointF> QPolygonFReverseIterator;


template <Qt::Orientation orientation>
class MLScanlineClipper
{
public:
	MLScanlineClipper(const QPolygonF &polygon);
	
	int reset();
	QList<QPolygonF> cilpNext();
	
private:
	
	void parsePolygon(const QPolygonF &polygon, QList<QPolygonF> &increasingPolygons, QList<QPolygonF> &decreasingPolygons);
	template <typename PolygonIterator> bool findIntersection(double &x, double y, PolygonIterator &i);
	
	double elemRow(const QPointF &p) { return orientation == Qt::Horizontal ? p.y() : p.x(); }
	double elemColumn(const QPointF &p) { return orientation == Qt::Horizontal ? p.x() : p.y(); }
	
	int _slMax, _slMin, _sl;
	
	QList<QPolygonF> _increasingPolygons;
	QList<QPolygonF> _decreasingPolygons;
	
	QList<QPolygonFIterator> _increaseIterators;
	QList<QPolygonFReverseIterator> _decreaseIterators;
};

template <Qt::Orientation orientation>
MLScanlineClipper<orientation>::MLScanlineClipper(const QPolygonF &polygon)
{
	parsePolygon(polygon, _increasingPolygons, _decreasingPolygons);
	
	int sectionCount = _increasingPolygons.size();
	
	// if polygon is split -decreasing- -increasing- -decreasing- -increasing- ... order
	// adjust polygon split to -increasing- -decreasing- ... order
	if (_increasingPolygons.at(0).at(0) != polygon.at(0))
	{
		QPolygonF p = _decreasingPolygons.at(0);
		
		for (int i = 1; i < _increasingPolygons.size(); ++i)
		{
			_decreasingPolygons[i-1] = _decreasingPolygons.at(i);
		}
		
		_decreasingPolygons[_decreasingPolygons.size() - 1] = p;
	}
	
	QList<double> slMaxCandidate, slMinCandidate;
	slMaxCandidate.reserve(sectionCount);
	slMinCandidate.reserve(sectionCount);
	
	_increaseIterators.reserve(sectionCount);
	_decreaseIterators.reserve(sectionCount);
	
	for (int i = 0; i < sectionCount; ++i)
	{
		slMaxCandidate << elemColumn(_increasingPolygons.at(i).last());
		slMinCandidate << elemColumn(_increasingPolygons.at(i).first());
		increaseIterators << QPolygonFIterator(_increasingPolygons.at(i));
		decreaseIterators << QPolygonFReverseIterator(_decreasingPolygons.at(i));
	}
	
	double slMaxDouble = *std::max_element(slMaxCandidate.begin(), slMaxCandidate.end());
	double slMinDouble = *std::min_element(slMinCandidate.begin(), slMinCandidate.end());
	
	_slMax = ceil(slMaxDouble);
	_slMin = floor(slMinDouble);
	
	for (int row = _slMin; row <= _slMax; ++i)
	{
		
	}
}

// Split the polygon into segments. In each segments, all y values of the points will be increasing or decreasing.
template <Qt::Orientation orientation>
void MLScanlineClipper<orientation>::parsePolygon(const QPolygonF &originalPolygon, QList<QPolygonF> &increasingPolygons, QList<QPolygonF> &decreasingPolygons)
{
	QPolygonF polygon = originalPolygon;
	if (polygon.first() != polygon.last())
		polygon << polygon.first();
	
	bool isIncreasing = elemColumn(polygon.at(1)) >= e_c(polygon.at(0));
	bool startIncreasing = isIncreasing;
	int prevIndex = 0;
	int count = polygon.size() - 1;
	int i;
	
	for (i = 1; i < count; ++i)
	{
		if (elemColumn(polygon.at(i+1)) >= elemColumn(polygon.at(i)))
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

template <Qt::Orientation orientation, typename PolygonIterator>
bool MLScanlineClipper<orientation>::findIntersection(double &x, double y, PolygonIterator &i)
{
	PolygonIterator back = i;
	back.toBack();
	
	if (back.peekPrevious().y() < y)
	{
		i = back;
		return false;
	}
	
	QPointF p1 = i.peekNext();
	
	if (p1.y() > y)
	{
		return false;
	}
	if (p1.y() == y)
	{
		x = p1.x();
		return true;
	}
	
	i.next();
	
	for (; i.hasNext(); i.next())
	{
		p1 = i.peekNext();
		
		if (p1.y() > y)
		{
			QPointF p0 = i.peekPrevious();
			x = ((y - p0.y()) * p1.x() + (p1.y() - y) * p0.x()) / (p1.y() - p0.y());
			i.previous();
			return true;
		}
		if (p1.y() == y)
		{
			x = p1.x();
			return true;
		}
	}
	
	Q_ASSERT(0);
	return false;
}

#endif // MLSCANLINECLIPPER_H
