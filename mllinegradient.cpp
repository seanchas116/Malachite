#include "mllinegradient.h"

float MLLineGradient::evaluate(float value) const
{
	int stopCount = _stops.size();
	if (stopCount == 0)
		return value;
	
	for (int i = 0; i < stopCount; ++i) {
		if (value <= _stops.keys().at(i)) {
			if (i != 0) {
				float x0 = _stops.keys().at(i-1);
				float x1 = _stops.keys().at(i);
				float y0 = _stops.values().at(i-1);
				float y1 = _stops.values().at(i);
				return y0 + (value - x0) / (x1 - x0) * (y1 - y0);
			} else {	// i == 0
				return _stops.values().at(0);
			}
		}
	}
	return _stops.values().at(stopCount - 1);
}
