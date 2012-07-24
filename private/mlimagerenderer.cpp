
#include "mlimagerenderer.h"

unsigned QPainterPath_vs::vertex(double *x, double *y)
{
	forever {
		
		unsigned type;
		if (_curve4) {	// curveの途中
			type = _curve4->vertex(x, y);
			if (type == agg::path_cmd_move_to)
				continue;	// 最初の頂点なので無視
			if (type != agg::path_cmd_stop)
				return type;	// line_to
			
			// ポリゴン終了
			delete _curve4;
			_curve4 = 0;
		}
		
		if (_index == _path.elementCount())	// path終わり
			return agg::path_cmd_stop;
		
		const QPainterPath::Element element = _path.elementAt(_index);
		
		if (element.type == QPainterPath::CurveToElement) {
			_curve4 = new agg::curve4_div(_path.elementAt(_index - 1).x, _path.elementAt(_index - 1).y,
									  _path.elementAt(_index).x, _path.elementAt(_index).y,
									  _path.elementAt(_index + 1).x, _path.elementAt(_index + 1).y,
									  _path.elementAt(_index + 2).x, _path.elementAt(_index + 2).y);
			_index += 3;
			continue;
		}
		
		_index++;
		*x = element.x;
		*y = element.y;
		
		switch (element.type) {
		case QPainterPath::MoveToElement:
			return agg::path_cmd_move_to;
		case QPainterPath::LineToElement:
			return agg::path_cmd_line_to;
		default:
			return agg::path_cmd_line_to;
		}
	}
}
