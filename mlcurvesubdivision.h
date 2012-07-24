#ifndef MLCURVESUBDIVISION_H
#define MLCURVESUBDIVISION_H

#include <QPolygonF>
#include "mlcurves.h"
#include "private/agg_array.h"

namespace agg
{

//-------------------------------------------------------------curve4_points
struct curve4_points
{
    double cp[8];
    curve4_points() {}
    curve4_points(double x1, double y1,
                  double x2, double y2,
                  double x3, double y3,
                  double x4, double y4)
    {
        cp[0] = x1; cp[1] = y1; cp[2] = x2; cp[3] = y2;
        cp[4] = x3; cp[5] = y3; cp[6] = x4; cp[7] = y4;
    }
    void init(double x1, double y1,
              double x2, double y2,
              double x3, double y3,
              double x4, double y4)
    {
        cp[0] = x1; cp[1] = y1; cp[2] = x2; cp[3] = y2;
        cp[4] = x3; cp[5] = y3; cp[6] = x4; cp[7] = y4;
    }
    double  operator [] (unsigned i) const { return cp[i]; }
    double& operator [] (unsigned i)       { return cp[i]; }
};


//-------------------------------------------------------------curve4_div
class curve4_div
{
public:
    curve4_div() : 
        m_approximation_scale(1.0),
        m_angle_tolerance(0.0),
        m_cusp_limit(0.0),
        m_count(0)
    {}

    curve4_div(double x1, double y1, 
               double x2, double y2, 
               double x3, double y3,
               double x4, double y4) :
        m_approximation_scale(1.0),
        m_angle_tolerance(0.0),
        m_cusp_limit(0.0),
        m_count(0)
    { 
        init(x1, y1, x2, y2, x3, y3, x4, y4);
    }

    curve4_div(const curve4_points& cp) :
        m_approximation_scale(1.0),
        m_angle_tolerance(0.0),
        m_count(0)
    { 
        init(cp[0], cp[1], cp[2], cp[3], cp[4], cp[5], cp[6], cp[7]);
    }

    void reset() { m_points.remove_all(); m_count = 0; }
    void init(double x1, double y1, 
              double x2, double y2, 
              double x3, double y3,
              double x4, double y4);

    void init(const curve4_points& cp)
    {
        init(cp[0], cp[1], cp[2], cp[3], cp[4], cp[5], cp[6], cp[7]);
    }

    void approximation_scale(double s) { m_approximation_scale = s; }
    double approximation_scale() const { return m_approximation_scale;  }

    void angle_tolerance(double a) { m_angle_tolerance = a; }
    double angle_tolerance() const { return m_angle_tolerance;  }

    void cusp_limit(double v) 
    { 
        m_cusp_limit = (v == 0.0) ? 0.0 : pi - v; 
    }

    double cusp_limit() const 
    { 
        return (m_cusp_limit == 0.0) ? 0.0 : pi - m_cusp_limit; 
    }

    void rewind(unsigned)
    {
        m_count = 0;
    }

    unsigned vertex(double* x, double* y)
    {
        if(m_count >= m_points.size()) return path_cmd_stop;
        const point_d& p = m_points[m_count++];
        *x = p.x;
        *y = p.y;
        return (m_count == 1) ? path_cmd_move_to : path_cmd_line_to;
    }
	
	int size() const
	{
		return m_points.size();
	}

private:
    void bezier(double x1, double y1, 
                double x2, double y2, 
                double x3, double y3, 
                double x4, double y4);

    void recursive_bezier(double x1, double y1, 
                          double x2, double y2, 
                          double x3, double y3, 
                          double x4, double y4,
                          unsigned level);

    double               m_approximation_scale;
    double               m_distance_tolerance_square;
    double               m_angle_tolerance;
    double               m_cusp_limit;
    unsigned             m_count;
    pod_bvector<point_d> m_points;
};

}

class MLCurveSubdivision
{
public:
	MLCurveSubdivision(const MLCurve4 &curve);
	
	QPolygonF polygon() const { return _polygon; }
	
private:
	MLCurve4 _curve;
	agg::curve4_div _curveSub;
	QPolygonF _polygon;
};

#endif // MLCURVESUBDIVISION_H
