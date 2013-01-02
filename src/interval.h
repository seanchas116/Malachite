#ifndef INTERVAL_H
#define INTERVAL_H

//ExportName: Interval

#include <algorithm>

namespace Malachite {

class Interval
{
public:
	
	Interval() : _start(0), _end(-1) {}
	Interval(int start, int length) : _start(start), _end(start + length - 1) {}
	
	int start() const { return _start; }
	int length() const { return _end - _start + 1; }
	int end() const { return _end; }
	
	bool isValid() const { return _end >= _start; }
	
	void setStart(int start) { _start = start; }
	void setEnd(int end) { _end = end; }
	void setLength(int len) { _end = _start + len - 1; }
	
	void shift(int diff)
	{
		_start += diff;
		_end += diff;
	}
	
	void moveStart(int start) { shift(start - _start); }
	void moveEnd(int end) { shift(end - _end); }
	
private:
	
	int _start, _end;
};

inline Interval operator|(const Interval &i1, const Interval &i2)
{
	Interval result;
	result.setStart(std::min(i1.start(), i2.start()));
	result.setEnd(std::max(i1.end(), i2.end()));
	return result;
}

inline Interval operator&(const Interval &i1, const Interval &i2)
{
	Interval result;
	result.setStart(std::max(i1.start(), i2.start()));
	result.setEnd(std::min(i1.end(), i2.end()));
	return result;
}

}

#endif // INTERVAL_H
