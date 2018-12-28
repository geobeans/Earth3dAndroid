#ifndef __ARROWPOINT_H__
#define __ARROWPOINT_H__
class ArrowPoint
{
public:
	ArrowPoint(void);
	ArrowPoint(double x, double y);
	~ArrowPoint(void);

   double x;
   double y;

public:
    ArrowPoint* subtract(ArrowPoint* p1);
    ArrowPoint* add(ArrowPoint* p1);
    double length();
    void normalize(double len);
    static ArrowPoint* interpolate(ArrowPoint* p1, ArrowPoint* p2, double r);
    static double distance(ArrowPoint* p1, ArrowPoint* p2);

    bool equals(ArrowPoint* p);
    ArrowPoint* clone() ;
    void set(ArrowPoint* p_);

};

#endif

